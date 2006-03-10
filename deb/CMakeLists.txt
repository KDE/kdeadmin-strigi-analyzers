kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kfile_deb_PART_SRCS kfile_deb.cpp )

kde4_automoc(${kfile_deb_PART_SRCS})

kde4_add_plugin(kfile_deb ${kfile_deb_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kfile_deb )

target_link_libraries(kfile_deb  ${QT_AND_KDECORE_LIBS} kio )

install_targets(${LIB_INSTALL_DIR}/kde4 kfile_deb )


########### install files ###############

install_files( ${SERVICES_INSTALL_DIR} FILES kfile_deb.desktop )

kde4_footer()



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
