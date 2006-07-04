
project(deb)



########### next target ###############

set(kfile_deb_PART_SRCS kfile_deb.cpp )

kde4_automoc(${kfile_deb_PART_SRCS})

kde4_add_plugin(kfile_deb ${kfile_deb_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kfile_deb )

target_link_libraries(kfile_deb  ${KDE4_KIO_LIBS} )

install(TARGETS kfile_deb  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES kfile_deb.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )




#original Makefile.am contents follow:

### Makefile.am for deb file meta info plugin
#
## set the include path for X, qt and KDE
#INCLUDES         = $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
## these are the headers for your project
#noinst_HEADERS   = kfile_deb.h
#
#kde_module_LTLIBRARIES = kfile_deb.la
#
#kfile_deb_la_SOURCES = kfile_deb.cpp
#kfile_deb_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
#kfile_deb_la_LIBADD = $(LIB_KSYCOCA)
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#messages:
#	$(XGETTEXT) *.cpp *.h -o $(podir)/kfile_deb.pot
#
#services_DATA = kfile_deb.desktop
#servicesdir = $(kde_servicesdir)
