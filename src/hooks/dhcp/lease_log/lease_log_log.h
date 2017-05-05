// Copyright (C) 2013-2015 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LEASE_LOG_LOG_H
#define LEASE_LOG_LOG_H

#include <log/message_initializer.h>
#include <log/macros.h>
#include <lease_log_messages.h>

namespace lease_log {

/// @brief User Check Logger
///
/// Define the logger used to log messages.  We could define it in multiple
/// modules, but defining in a single module and linking to it saves time and
/// space.
extern isc::log::Logger lease_log_logger;

} // end of namespace lease_log

#endif // LEASE_LOG_LOG_H
