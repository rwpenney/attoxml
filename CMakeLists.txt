# CMake control-file for minixml document generator
# RW Penney, May 2012

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(minixml)

IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-ansi -pedantic -Wall)
    SET(CMAKE_CXX_FLAGS_DEBG:STRING "-ggdb")
ENDIF(CMAKE_COMPILER_IS_GNUCC)


SET(lib_hdrs
    minixml.h
)

SET(lib_srcs
    minixml.cpp
)


ADD_LIBRARY(minixml STATIC ${lib_hdrs} ${lib_srcs})

ADD_EXECUTABLE(demo ${lib_hdrs} ${lib_srcs})
SET_TARGET_PROPERTIES(demo
    PROPERTIES COMPILE_FLAGS "-DDEMO_MAIN")


# vim: set ts=4 sw=4 et:
