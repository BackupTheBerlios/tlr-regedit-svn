PROJECT(regedit)

FIND_FILE(registry_header registry.h /usr/include)

IF (NOT registry_header)
	MESSAGE(FATAL_ERROR "can not find file registry.h")
ELSE (NOT registry_header)
	MESSAGE(STATUS "found registry.h")
ENDIF (NOT registry_header)

FIND_LIBRARY(libregistry NAMES registry PATHS /usr/lib /lib)

IF (NOT libregistry)
	MESSAGE(FATAL_ERROR "can not find the registry library")
ELSE (NOT libregistry)
	MESSAGE(STATUS "found registry library")
ENDIF (NOT libregistry)

INCLUDE(${CMAKE_ROOT}/Modules/FindQt.cmake)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)

SUBDIRS(src)

ADD_CUSTOM_TARGET(svnexport
svn export ${CMAKE_CURRENT_SOURCE_DIR}/ /tmp/regedit
)

ADD_CUSTOM_TARGET(targz
tar -czf regedit.tar.gz /tmp/regedit
DEPENDS svnexport
)

ADD_CUSTOM_TARGET(svndist
rm -R /tmp/regedit
DEPENDS targz
)