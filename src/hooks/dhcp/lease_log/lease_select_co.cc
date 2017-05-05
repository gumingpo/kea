// Copyright (C) 2013-2015 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// @file subnet_select_co.cc Defines the subnet4_select and subnet6_select callout functions.

#include <config.h>
#include <hooks/hooks.h>
#include <dhcp/pkt4.h>
#include <dhcp/dhcp6.h>
#include <dhcp/pkt6.h>
#include <dhcpsrv/subnet.h>
#include <lease_log_log.h>
#include <udp_client.h>

using namespace isc::dhcp;
using namespace isc::hooks;
using namespace lease_log;
using namespace std;

// Functions accessed by the hooks framework use C linkage to avoid the name
// mangling that accompanies use of the C++ compiler as well as to avoid
// issues related to namespaces.
extern "C" {
	int lease4_select(CalloutHandle& handle) {
		try {
			// Get subnet collection. If it's empty just bail nothing to do.
			isc::dhcp::Lease4Ptr lease4;
			handle.getArgument("lease4", lease4);

			bool fake_allocation;
			handle.getArgument("fake_allocation",fake_allocation);

			char buf[BUFLEN] = "";
			sprintf(buf,"%s,%s,lease4_select",lease4->addr_.toText().c_str(), lease4->hwaddr_->toText().c_str());
			LOG_INFO(lease_log_logger, "[lease4_select] lease:%1, fake_allocation:%2").arg(buf).arg(fake_allocation);
			if(fake_allocation == false) {
				LOG_INFO(lease_log_logger, "[lease4_select] to notify lease:%1").arg(buf);
				udp_send(buf,strlen(buf), udpServer.server, udpServer.port);
			}
		} catch (const exception& ex) {
			LOG_ERROR(lease_log_logger, LEASE_LOG_SUBNET4_SELECT_ERROR)
					.arg(ex.what());
			return (1);
		}
		return (0);
	}
	int lease4_renew(CalloutHandle& handle) {
		try {
			// Get subnet collection. If it's empty just bail nothing to do.
			isc::dhcp::Lease4Ptr lease4;
			handle.getArgument("lease4", lease4);

			char buf[BUFLEN] = "";
			sprintf(buf,"%s,%s,lease4_renew",lease4->addr_.toText().c_str(), lease4->hwaddr_->toText().c_str());
			LOG_INFO(lease_log_logger, "[lease4_renew] lease:%1").arg(buf);
	        udp_send(buf,strlen(buf), udpServer.server, udpServer.port);
		} catch (const exception& ex) {
			LOG_ERROR(lease_log_logger, LEASE_LOG_SUBNET4_SELECT_ERROR)
					.arg(ex.what());
			return (1);
		}
		return (0);
	}
	int lease4_release(CalloutHandle& handle) {
		try {
			// Get subnet collection. If it's empty just bail nothing to do.
			isc::dhcp::Lease4Ptr lease4;
			handle.getArgument("lease4", lease4);

			char buf[BUFLEN] = "";
			sprintf(buf,"%s,%s,lease4_release",lease4->addr_.toText().c_str(), lease4->hwaddr_->toText().c_str());
			LOG_INFO(lease_log_logger, "[lease4_release] lease:%1").arg(buf);
//	        udp_send(buf,strlen(buf), udpServer.server, udpServer.port);
		} catch (const exception& ex) {
			LOG_ERROR(lease_log_logger, LEASE_LOG_SUBNET4_SELECT_ERROR)
					.arg(ex.what());
			return (1);
		}
		return (0);
	}

	int lease4_decline(CalloutHandle& handle) {
		try {
			// Get subnet collection. If it's empty just bail nothing to do.
			isc::dhcp::Lease4Ptr lease4;
			handle.getArgument("lease4", lease4);

			char buf[BUFLEN] = "";
			sprintf(buf,"%s,%s,lease4_decline",lease4->addr_.toText().c_str(), lease4->hwaddr_->toText().c_str());
			LOG_INFO(lease_log_logger, "[lease4_decline] lease:%1").arg(buf);
//	        udp_send(buf,strlen(buf), udpServer.server, udpServer.port);
		} catch (const exception& ex) {
			LOG_ERROR(lease_log_logger, LEASE_LOG_SUBNET4_SELECT_ERROR)
					.arg(ex.what());
			return (1);
		}
		return (0);
	}

	int lease4_expire(CalloutHandle& handle) {
		try {
			// Get subnet collection. If it's empty just bail nothing to do.
			isc::dhcp::Lease4Ptr lease4;
			handle.getArgument("lease4", lease4);

			char buf[BUFLEN] = "";
			sprintf(buf,"%s,%s,lease4_expire",lease4->addr_.toText().c_str(), lease4->hwaddr_->toText().c_str());
			LOG_INFO(lease_log_logger, "[lease4_expire] lease:%1").arg(buf);
//	        udp_send(buf,strlen(buf), udpServer.server, udpServer.port);
		} catch (const exception& ex) {
			LOG_ERROR(lease_log_logger, LEASE_LOG_SUBNET4_SELECT_ERROR)
					.arg(ex.what());
			return (1);
		}
		return (0);
	}
}
