SUBDIRS = . tests

AM_CPPFLAGS  = -I$(top_builddir)/src/lib -I$(top_srcdir)/src/lib
AM_CPPFLAGS += $(BOOST_INCLUDES)
AM_CXXFLAGS  = $(KEA_CXXFLAGS)

# Define rule to build logging source files from message file
user_chk_messages.h user_chk_messages.cc: s-messages
s-messages: user_chk_messages.mes
	$(top_builddir)/src/lib/log/compiler/kea-msg-compiler $(top_srcdir)/src/hooks/dhcp/user_chk/user_chk_messages.mes
	touch $@

# Tell automake that the message files are built as part of the build process
# (so that they are built before the main library is built).
BUILT_SOURCES = user_chk_messages.h user_chk_messages.cc

EXTRA_DIST = libdhcp_user_chk.dox
# Ensure that the message file is included in the distribution
EXTRA_DIST += user_chk_messages.mes

# Get rid of generated message files on a clean
CLEANFILES = *.gcno *.gcda user_chk_messages.h user_chk_messages.cc s-messages

lib_LTLIBRARIES = libdhcp_user_chk.la

libdhcp_user_chk_la_SOURCES =
libdhcp_user_chk_la_SOURCES += udp_client.h udp_client.cc
libdhcp_user_chk_la_SOURCES += load_unload.cc
libdhcp_user_chk_la_SOURCES += pkt_receive_co.cc
libdhcp_user_chk_la_SOURCES += pkt_send_co.cc
libdhcp_user_chk_la_SOURCES += subnet_select_co.cc
libdhcp_user_chk_la_SOURCES += user.cc user.h
libdhcp_user_chk_la_SOURCES += user_chk.h
libdhcp_user_chk_la_SOURCES += user_chk_log.cc user_chk_log.h
libdhcp_user_chk_la_SOURCES += user_data_source.h
libdhcp_user_chk_la_SOURCES += user_file.cc user_file.h
libdhcp_user_chk_la_SOURCES += user_registry.cc user_registry.h
libdhcp_user_chk_la_SOURCES += version.cc
libdhcp_user_chk_la_SOURCES += user_chk_messages.cc user_chk_messages.h
libdhcp_user_chk_la_CXXFLAGS = $(AM_CXXFLAGS)
libdhcp_user_chk_la_CPPFLAGS = $(AM_CPPFLAGS)
libdhcp_user_chk_la_LIBADD  = $(top_builddir)/src/lib/hooks/libkea-hooks.la
libdhcp_user_chk_la_LIBADD  += $(top_builddir)/src/lib/log/libkea-log.la
libdhcp_user_chk_la_LIBADD  += $(top_builddir)/src/lib/dhcpsrv/libkea-dhcpsrv.la
libdhcp_user_chk_la_LIBADD  += $(top_builddir)/src/lib/dhcp/libkea-dhcp++.la
libdhcp_user_chk_la_LIBADD  += $(top_builddir)/src/lib/util/threads/libkea-threads.la
libdhcp_user_chk_la_LIBADD  += $(top_builddir)/src/lib/util/libkea-util.la
libdhcp_user_chk_la_LIBADD  += $(top_builddir)/src/lib/exceptions/libkea-exceptions.la
libdhcp_user_chk_la_LIBADD  += $(LOG4CPLUS_LIBS)
libdhcp_user_chk_la_LDFLAGS = -no-undefined -version-info 2:0:0
libdhcp_user_chk_la_LDFLAGS += $(CRYPTO_LDFLAGS)
