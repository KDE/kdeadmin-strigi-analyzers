## Makefile.am for deb file meta info plugin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = kfile_deb.h

kde_module_LTLIBRARIES = kfile_deb.la

kfile_deb_la_SOURCES = kfile_deb.cpp
kfile_deb_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
kfile_deb_la_LIBADD = $(LIB_KSYCOCA)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages:
	$(XGETTEXT) *.cpp *.h -o $(podir)/kfile_deb.pot

services_DATA = kfile_deb.desktop
servicesdir = $(kde_servicesdir)
