# CMake control-file for attoxml document generator
# RW Penney, May 2012

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(ATTO_VERSION "0.1")
PROJECT(attoxml)

IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-ansi -pedantic -Wall)
    SET(CMAKE_CXX_FLAGS_DEBG:STRING "-ggdb")
ENDIF(CMAKE_COMPILER_IS_GNUCC)


SET(lib_hdrs
    attoxml.h
)

SET(lib_srcs
    attoxml.cpp
)

SET(EXTRA_DIST CMakeLists.txt)


ADD_LIBRARY(attoxml STATIC ${lib_hdrs} ${lib_srcs})

ADD_EXECUTABLE(demo ${lib_hdrs} ${lib_srcs})
SET_TARGET_PROPERTIES(demo
    PROPERTIES COMPILE_FLAGS "-DATTO_DEMO_MAIN")


ADD_CUSTOM_TARGET(dist
    COMMAND ${CMAKE_COMMAND} -E tar zcf attoxml-${ATTO_VERSION}.tgz
            ${lib_hdrs} ${lib_srcs} ${EXTRA_DIST})

# vim: set ts=4 sw=4 et:
