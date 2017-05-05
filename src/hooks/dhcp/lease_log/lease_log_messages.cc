// File created from ../../../../src/hooks/dhcp/lease_log/lease_log_messages.mes on Fri May 05 2017 17:21

#include <cstddef>
#include <log/message_types.h>
#include <log/message_initializer.h>

extern const isc::log::MessageID LEASE_LOG_HOOK_LOAD_ERROR = "LEASE_LOG_HOOK_LOAD_ERROR";
extern const isc::log::MessageID LEASE_LOG_HOOK_UNLOAD_ERROR = "LEASE_LOG_HOOK_UNLOAD_ERROR";
extern const isc::log::MessageID LEASE_LOG_SUBNET4_SELECT_ERROR = "LEASE_LOG_SUBNET4_SELECT_ERROR";
extern const isc::log::MessageID LEASE_LOG_SUBNET4_SELECT_REGISTRY_NULL = "LEASE_LOG_SUBNET4_SELECT_REGISTRY_NULL";
extern const isc::log::MessageID LEASE_LOG_SUBNET6_SELECT_ERROR = "LEASE_LOG_SUBNET6_SELECT_ERROR";
extern const isc::log::MessageID LEASE_LOG_SUBNET6_SELECT_REGISTRY_NULL = "LEASE_LOG_SUBNET6_SELECT_REGISTRY_NULL";

namespace {

const char* values[] = {
    "LEASE_LOG_HOOK_LOAD_ERROR", "DHCP UserCheckHook could not be loaded: %1",
    "LEASE_LOG_HOOK_UNLOAD_ERROR", "DHCP UserCheckHook an error occurred unloading the library: %1",
    "LEASE_LOG_SUBNET4_SELECT_ERROR", "DHCP UserCheckHook an unexpected error occurred in subnet4_select callout: %1",
    "LEASE_LOG_SUBNET4_SELECT_REGISTRY_NULL", "DHCP UserCheckHook UserRegistry has not been created.",
    "LEASE_LOG_SUBNET6_SELECT_ERROR", "DHCP UserCheckHook an unexpected error occurred in subnet6_select callout: %1",
    "LEASE_LOG_SUBNET6_SELECT_REGISTRY_NULL", "DHCP UserCheckHook UserRegistry has not been created.",
    NULL
};

const isc::log::MessageInitializer initializer(values);

} // Anonymous namespace

