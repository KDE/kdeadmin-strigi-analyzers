## Makefile.am for rpm file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = kfile_rpm.h

kde_module_LTLIBRARIES = kfile_rpm.la

kfile_rpm_la_SOURCES = kfile_rpm.cpp
kfile_rpm_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
kfile_rpm_la_LIBADD = $(LIB_KSYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp *.h -o $(podir)/kfile_rpm.pot

services_DATA = kfile_rpm.desktop
servicesdir = $(kde_servicesdir)
