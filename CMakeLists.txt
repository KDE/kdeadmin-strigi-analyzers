
MESSAGE(STATUS "rpm and deb file plugins necessary in system != UNIX ?")
MESSAGE(STATUS "PORT kfile plugins!!!")
if(KFILE_PLUGINS_PORTED)
add_subdirectory( rpm ) 
endif(KFILE_PLUGINS_PORTED)
