kde4_header()
project(rpm)
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kfile_rpm_PART_SRCS kfile_rpm.cpp )

kde4_automoc(${kfile_rpm_PART_SRCS})

kde4_add_plugin(kfile_rpm ${kfile_rpm_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kfile_rpm )

target_link_libraries(kfile_rpm  ${KDE4_KIO_LIBS} )

install_targets(${PLUGIN_INSTALL_DIR} kfile_rpm )


########### install files ###############

install_files( ${SERVICES_INSTALL_DIR} FILES kfile_rpm.desktop )

kde4_footer()



#original Makefile.am contents follow:

### Makefile.am for rpm file meta info plugin
#
## set the include path for X, qt and KDE
#INCLUDES         = $(all_includes)
#
## these are the headers for your project
#noinst_HEADERS   = kfile_rpm.h
#
#kde_module_LTLIBRARIES = kfile_rpm.la
#
#kfile_rpm_la_SOURCES = kfile_rpm.cpp
#kfile_rpm_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
#kfile_rpm_la_LIBADD = $(LIB_KSYCOCA)
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#messages:
#	$(XGETTEXT) *.cpp *.h -o $(podir)/kfile_rpm.pot
#
#services_DATA = kfile_rpm.desktop
#servicesdir = $(kde_servicesdir)
