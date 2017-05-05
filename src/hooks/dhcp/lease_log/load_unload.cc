// Copyright (C) 2013-2015 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// @file load_unload.cc Defines the load and unload hooks library functions.

#include <config.h>

#include <hooks/hooks.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include "lease_log_log.h"
#include "udp_client.h"
#include <string.h>
#include <stdio.h>
using namespace isc::hooks;

// Functions accessed by the hooks framework use C linkage to avoid the name
// mangling that accompanies use of the C++ compiler as well as to avoid
// issues related to namespaces.
extern "C" {

	/// @brief Called by the Hooks library manager when the library is loaded.
	/// @return Returns 0 upon success, non-zero upon failure.
	int load(LibraryHandle& handle) {
		isc::data::ConstElementPtr serverElement =  handle.getParameter("server");
		if(!serverElement) return 1;
		if(serverElement->getType() != isc::data::Element::string) return 1;

		isc::data::ConstElementPtr portElement = handle.getParameter("port");
		if(!portElement) return 1;
		if(portElement->getType() != isc::data::Element::integer) return 1;

		const char* server = (serverElement->stringValue()).c_str();
        int port = portElement->intValue();
        snprintf(udpServer.server, strlen("255.255.255.255"), "%s",server);
        udpServer.port = port;
        std::cout << "lease_log started with udp server:" << udpServer.server << ", port:" << udpServer.port << std::endl;
		return 0;
	}

	/// @brief Called by the Hooks library manager when the library is unloaded.
	/// @return Always returns 0.
	int unload() {
		return (0);
	}
}
