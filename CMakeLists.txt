
MESSAGE(STATUS "rpm and deb file plugins necessary in system != UNIX ?")
add_subdirectory( rpm ) 
add_subdirectory( deb ) 




########### install files ###############





#original Makefile.am contents follow:

#SUBDIRS=rpm deb
