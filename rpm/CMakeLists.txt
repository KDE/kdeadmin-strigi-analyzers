project(rpm)



########### next target ###############

set(kfile_rpm_PART_SRCS kfile_rpm.cpp )

kde4_add_plugin(kfile_rpm ${kfile_rpm_PART_SRCS})



target_link_libraries(kfile_rpm  ${KDE4_KIO_LIBS} )

install(TARGETS kfile_rpm  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

install( FILES kfile_rpm.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )

