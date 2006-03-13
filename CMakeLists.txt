kde4_header()

MESSAGE(STATUS "rpm and deb file plugins necessary in system != UNIX ?")
add_subdirectory( rpm ) 
add_subdirectory( deb ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS=rpm deb
