
MESSAGE(STATUS "rpm and deb file plugins necessary in system != UNIX ?")
MESSAGE(STATUS "PORT kfile plugins!!!")
if(KFILE_PLUGINS_PORTED)
add_subdirectory( rpm ) 
add_subdirectory( deb ) 
endif(KFILE_PLUGINS_PORTED)
