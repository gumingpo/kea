SUBDIRS = .

AM_CPPFLAGS  = -I$(top_builddir)/src/lib -I$(top_srcdir)/src/lib
AM_CPPFLAGS += $(BOOST_INCLUDES)
AM_CXXFLAGS  = $(KEA_CXXFLAGS)

# Define rule to build logging source files from message file
lease_log_messages.h lease_log_messages.cc: s-messages
s-messages: lease_log_messages.mes
	$(top_builddir)/src/lib/log/compiler/kea-msg-compiler $(top_srcdir)/src/hooks/dhcp/lease_log/lease_log_messages.mes
	touch $@

# Tell automake that the message files are built as part of the build process
# (so that they are built before the main library is built).
BUILT_SOURCES = lease_log_messages.h lease_log_messages.cc

EXTRA_DIST = libdhcp_lease_log.dox
# Ensure that the message file is included in the distribution
EXTRA_DIST += lease_log_messages.mes

# Get rid of generated message files on a clean
CLEANFILES = *.gcno *.gcda lease_log_messages.h lease_log_messages.cc s-messages

lib_LTLIBRARIES = libdhcp_lease_log.la

libdhcp_lease_log_la_SOURCES =
libdhcp_lease_log_la_SOURCES += version.cc load_unload.cc
libdhcp_lease_log_la_SOURCES += udp_client.h udp_client.cc
libdhcp_lease_log_la_SOURCES += lease_log_log.cc lease_log_log.h lease_log_messages.cc lease_log_messages.h
libdhcp_lease_log_la_SOURCES += lease_select_co.cc
libdhcp_lease_log_la_CXXFLAGS = $(AM_CXXFLAGS)
libdhcp_lease_log_la_CPPFLAGS = $(AM_CPPFLAGS)
libdhcp_lease_log_la_LIBADD  = $(top_builddir)/src/lib/hooks/libkea-hooks.la
libdhcp_lease_log_la_LIBADD  += $(top_builddir)/src/lib/log/libkea-log.la
libdhcp_lease_log_la_LIBADD  += $(top_builddir)/src/lib/dhcpsrv/libkea-dhcpsrv.la
libdhcp_lease_log_la_LIBADD  += $(top_builddir)/src/lib/dhcp/libkea-dhcp++.la
libdhcp_lease_log_la_LIBADD  += $(top_builddir)/src/lib/util/threads/libkea-threads.la
libdhcp_lease_log_la_LIBADD  += $(top_builddir)/src/lib/util/libkea-util.la
libdhcp_lease_log_la_LIBADD  += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
libdhcp_lease_log_la_LIBADD  += $(LOG4CPLUS_LIBS)
libdhcp_lease_log_la_LDFLAGS = -no-undefined -version-info 2:0:0
libdhcp_lease_log_la_LDFLAGS += $(CRYPTO_LDFLAGS)
