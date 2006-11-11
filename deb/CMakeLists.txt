
project(deb)



########### next target ###############

set(kfile_deb_PART_SRCS kfile_deb.cpp )

kde4_automoc(${kfile_deb_PART_SRCS})

kde4_add_plugin(kfile_deb ${kfile_deb_PART_SRCS})



target_link_libraries(kfile_deb  ${KDE4_KIO_LIBS} )

install(TARGETS kfile_deb  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES kfile_deb.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )

