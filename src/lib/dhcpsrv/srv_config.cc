// Copyright (C) 2014-2017 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <dhcpsrv/cfgmgr.h>
#include <dhcpsrv/srv_config.h>
#include <dhcpsrv/lease_mgr_factory.h>
#include <dhcpsrv/cfg_hosts_util.h>
#include <log/logger_manager.h>
#include <log/logger_specification.h>
#include <dhcp/pkt.h> // Needed for HWADDR_SOURCE_*
#include <list>
#include <sstream>

using namespace isc::log;
using namespace isc::data;

namespace isc {
namespace dhcp {

SrvConfig::SrvConfig()
    : sequence_(0), cfg_iface_(new CfgIface()),
      cfg_option_def_(new CfgOptionDef()), cfg_option_(new CfgOption()),
      cfg_subnets4_(new CfgSubnets4()), cfg_subnets6_(new CfgSubnets6()),
      cfg_hosts_(new CfgHosts()), cfg_rsoo_(new CfgRSOO()),
      cfg_expiration_(new CfgExpiration()), cfg_duid_(new CfgDUID()),
      cfg_db_access_(new CfgDbAccess()),
      cfg_host_operations4_(CfgHostOperations::createConfig4()),
      cfg_host_operations6_(CfgHostOperations::createConfig6()),
      class_dictionary_(new ClientClassDictionary()),
      decline_timer_(0), echo_v4_client_id_(true), dhcp4o6_port_(0),
      d2_client_config_(new D2ClientConfig()) {
}

SrvConfig::SrvConfig(const uint32_t sequence)
    : sequence_(sequence), cfg_iface_(new CfgIface()),
      cfg_option_def_(new CfgOptionDef()), cfg_option_(new CfgOption()),
      cfg_subnets4_(new CfgSubnets4()), cfg_subnets6_(new CfgSubnets6()),
      cfg_hosts_(new CfgHosts()), cfg_rsoo_(new CfgRSOO()),
      cfg_expiration_(new CfgExpiration()), cfg_duid_(new CfgDUID()),
      cfg_db_access_(new CfgDbAccess()),
      cfg_host_operations4_(CfgHostOperations::createConfig4()),
      cfg_host_operations6_(CfgHostOperations::createConfig6()),
      class_dictionary_(new ClientClassDictionary()),
      decline_timer_(0), echo_v4_client_id_(true), dhcp4o6_port_(0),
      d2_client_config_(new D2ClientConfig()) {
}

std::string
SrvConfig::getConfigSummary(const uint32_t selection) const {
    std::ostringstream s;
    size_t subnets_num;
    if ((selection & CFGSEL_SUBNET4) == CFGSEL_SUBNET4) {
        subnets_num = getCfgSubnets4()->getAll()->size();
        if (subnets_num > 0) {
            s << "added IPv4 subnets: " << subnets_num;
        } else {
            s << "no IPv4 subnets!";
        }
        s << "; ";
    }

    if ((selection & CFGSEL_SUBNET6) == CFGSEL_SUBNET6) {
        subnets_num = getCfgSubnets6()->getAll()->size();
        if (subnets_num > 0) {
            s << "added IPv6 subnets: " << subnets_num;
        } else {
            s << "no IPv6 subnets!";
        }
        s << "; ";
    }

    if ((selection & CFGSEL_DDNS) == CFGSEL_DDNS) {
        bool ddns_enabled = getD2ClientConfig()->getEnableUpdates();
        s << "DDNS: " << (ddns_enabled ? "enabled" : "disabled") << "; ";
    }

    if (s.tellp() == static_cast<std::streampos>(0)) {
        s << "no config details available";
    }

    std::string summary = s.str();
    size_t last_separator_pos = summary.find_last_of(";");
    if (last_separator_pos == summary.length() - 2) {
        summary.erase(last_separator_pos);
    }
    return (summary);
}

bool
SrvConfig::sequenceEquals(const SrvConfig& other) {
    return (getSequence() == other.getSequence());
}

void
SrvConfig::copy(SrvConfig& new_config) const {
    // We will entirely replace loggers in the new configuration.
    new_config.logging_info_.clear();
    for (LoggingInfoStorage::const_iterator it = logging_info_.begin();
         it != logging_info_.end(); ++it) {
        new_config.addLoggingInfo(*it);
    }
    // Replace interface configuration.
    new_config.cfg_iface_.reset(new CfgIface(*cfg_iface_));
    // Replace option definitions.
    cfg_option_def_->copyTo(*new_config.cfg_option_def_);
    cfg_option_->copyTo(*new_config.cfg_option_);
    // Replace the client class dictionary
    new_config.class_dictionary_.reset(new ClientClassDictionary(*class_dictionary_));
    // Replace the D2 client configuration
    new_config.setD2ClientConfig(getD2ClientConfig());
    // Replace configured hooks libraries.
    new_config.hooks_config_.clear();
    using namespace isc::hooks;
    for (HookLibsCollection::const_iterator it =
           hooks_config_.get().begin();
         it != hooks_config_.get().end(); ++it) {
        new_config.hooks_config_.add(it->first, it->second);
    }
}

void
SrvConfig::applyLoggingCfg() const {

    std::list<LoggerSpecification> specs;
    for (LoggingInfoStorage::const_iterator it = logging_info_.begin();
         it != logging_info_.end(); ++it) {
        specs.push_back(it->toSpec());
    }
    LoggerManager manager;
    manager.process(specs.begin(), specs.end());
}

bool
SrvConfig::equals(const SrvConfig& other) const {
    // If number of loggers is different, then configurations aren't equal.
    if (logging_info_.size() != other.logging_info_.size()) {
        return (false);
    }
    // Pass through all loggers and try to find the match for each of them
    // with the loggers from the other configuration. The order doesn't
    // matter so we can't simply compare the vectors.
    for (LoggingInfoStorage::const_iterator this_it =
             logging_info_.begin(); this_it != logging_info_.end();
         ++this_it) {
        bool match = false;
        for (LoggingInfoStorage::const_iterator other_it =
                 other.logging_info_.begin();
             other_it != other.logging_info_.end(); ++other_it) {
            if (this_it->equals(*other_it)) {
                match = true;
                break;
            }
        }
        // No match found for the particular logger so return false.
        if (!match) {
            return (false);
        }
    }
    // Logging information is equal between objects, so check other values.
    if ((*cfg_iface_ != *other.cfg_iface_) ||
        (*cfg_option_def_ != *other.cfg_option_def_) ||
        (*cfg_option_ != *other.cfg_option_) ||
        (*class_dictionary_ != *other.class_dictionary_) ||
        (*d2_client_config_ != *other.d2_client_config_)) {
        return (false);
    }
    // Now only configured hooks libraries can differ.
    // If number of configured hooks libraries are different, then
    // configurations aren't equal.
    if (hooks_config_.get().size() != other.hooks_config_.get().size()) {
        return (false);
    }
    // Pass through all configured hooks libraries.
    return (hooks_config_.equal(other.hooks_config_));
}

void
SrvConfig::removeStatistics() {

    // Removes statistics for v4 and v6 subnets
    getCfgSubnets4()->removeStatistics();

    getCfgSubnets6()->removeStatistics();
}

void
SrvConfig::updateStatistics() {
    // Updating subnet statistics involves updating lease statistics, which
    // is done by the LeaseMgr.  Since servers with subnets, must have a
    // LeaseMgr, we do not bother updating subnet stats for servers without
    // a lease manager, such as D2. @todo We should probably examine why
    // "SrvConfig" is being used by D2.
    if (LeaseMgrFactory::haveInstance()) {
        // Updates  statistics for v4 and v6 subnets
        getCfgSubnets4()->updateStatistics();

        getCfgSubnets6()->updateStatistics();
    }
}

ElementPtr
SrvConfig::toElement() const {
    // Get family for the configuration manager
    uint16_t family = CfgMgr::instance().getFamily();
    // Toplevel map
    ElementPtr result = Element::createMap();
    // DhcpX global map
    ElementPtr dhcp = Element::createMap();
    // Set decline-probation-period
    dhcp->set("decline-probation-period",
              Element::create(static_cast<long long>(decline_timer_)));
    // Set echo-client-id (DHCPv4)
    if (family == AF_INET) {
        dhcp->set("echo-client-id", Element::create(echo_v4_client_id_));
    }
    // Set dhcp4o6-port
    dhcp->set("dhcp4o6-port",
              Element::create(static_cast<int>(dhcp4o6_port_)));
    // Set dhcp-ddns
    dhcp->set("dhcp-ddns", d2_client_config_->toElement());
    // Set interfaces-config
    dhcp->set("interfaces-config", cfg_iface_->toElement());
    // Set option-def
    dhcp->set("option-def", cfg_option_def_->toElement());
    // Set option-data
    dhcp->set("option-data", cfg_option_->toElement());
    // Set subnets
    ConstElementPtr subnets;
    if (family == AF_INET) {
        subnets = cfg_subnets4_->toElement();
        dhcp->set("subnet4", subnets);
    } else {
        subnets = cfg_subnets6_->toElement();
        dhcp->set("subnet6", subnets);
    }
    // Insert reservations
    CfgHostsList resv_list;
    resv_list.internalize(cfg_hosts_->toElement());
    const std::vector<ElementPtr>& sn_list = subnets->listValue();
    for (std::vector<ElementPtr>::const_iterator subnet = sn_list.begin();
         subnet != sn_list.end(); ++subnet) {
        ConstElementPtr id = (*subnet)->get("id");
        if (isNull(id)) {
            isc_throw(ToElementError, "subnet has no id");
        }
        SubnetID subnet_id = id->intValue();
        ConstElementPtr resvs = resv_list.get(subnet_id);
        (*subnet)->set("reservations", resvs);
    }
    // Set expired-leases-processing
    ConstElementPtr expired = cfg_expiration_->toElement();
    dhcp->set("expired-leases-processing", expired);
    if (family == AF_INET6) {
        // Set server-id (DHCPv6)
        dhcp->set("server-id", cfg_duid_->toElement());

        // Set relay-supplied-options (DHCPv6)
        dhcp->set("relay-supplied-options", cfg_rsoo_->toElement());
    }
    // Set lease-database
    CfgLeaseDbAccess lease_db(*cfg_db_access_);
    dhcp->set("lease-database", lease_db.toElement());
    // Set hosts-database
    CfgHostDbAccess host_db(*cfg_db_access_);
    // @todo accept empty map
    ConstElementPtr hosts_database = host_db.toElement();
    if (hosts_database->size() > 0) {
        dhcp->set("hosts-database", hosts_database);
    }
    // Set host-reservation-identifiers
    ConstElementPtr host_ids;
    if (family == AF_INET) {
        host_ids = cfg_host_operations4_->toElement();
    } else {
        host_ids = cfg_host_operations6_->toElement();
    }
    dhcp->set("host-reservation-identifiers", host_ids);
    // Set mac-sources (DHCPv6)
    if (family == AF_INET6) {
        dhcp->set("mac-sources", cfg_mac_source_.toElement());
    }
    // Set control-socket (skip if null as empty is not legal)
    if (!isNull(control_socket_)) {
        dhcp->set("control-socket", control_socket_);
    }
    // Set client-classes
    ConstElementPtr client_classes = class_dictionary_->toElement();
    // @todo accept empty list
    if (!client_classes->empty()) {
        dhcp->set("client-classes", client_classes);
    }
    // Set hooks-libraries
    ConstElementPtr hooks_libs = hooks_config_.toElement();
    dhcp->set("hooks-libraries", hooks_libs);
    // Set DhcpX
    result->set(family == AF_INET ? "Dhcp4" : "Dhcp6", dhcp);

    // Logging global map (skip if empty)
    if (!logging_info_.empty()) {
        ElementPtr logging = Element::createMap();
        // Set loggers list
        ElementPtr loggers = Element::createList();
        for (LoggingInfoStorage::const_iterator logger =
                 logging_info_.cbegin();
             logger != logging_info_.cend(); ++logger) {
            loggers->add(logger->toElement());
        }
        logging->set("loggers", loggers);
        result->set("Logging", logging);
    }

    return (result);
}

}
}
