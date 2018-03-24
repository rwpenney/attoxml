# CMake control-file for attoxml document generator
# RW Penney, May 2012

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(ATTO_VERSION "0.3.1")
PROJECT(attoxml)

IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-ansi -pedantic -Wall)
    SET(CMAKE_CXX_FLAGS_DEBUG:STRING "-ggdb")
ENDIF(CMAKE_COMPILER_IS_GNUCC)


FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf @ONLY)
    SET(DOXY_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)
    ADD_CUSTOM_TARGET(doxydoc ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG})
ENDIF(DOXYGEN_FOUND)



SET(lib_hdrs
    attoxml.h
)

SET(lib_srcs
    attoxml.cpp
)

SET(EXTRA_DIST CMakeLists.txt LICENSE.txt README.txt example.cpp)


ADD_LIBRARY(attoxml SHARED ${lib_hdrs} ${lib_srcs})
SET_TARGET_PROPERTIES(attoxml PROPERTIES SOVERSION "${ATTO_VERSION}")
INSTALL(TARGETS attoxml LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib RUNTIME DESTINATION lib)
INSTALL(FILES ${lib_hdrs} DESTINATION include)


ADD_EXECUTABLE(attodemo ${lib_hdrs} ${lib_srcs} example.cpp)


IF(UNIX)
    ADD_CUSTOM_TARGET(dist
        COMMAND mkdir -p attoxml-${ATTO_VERSION}
        COMMAND cp -p ${lib_hdrs} ${lib_srcs} ${EXTRA_DIST}
                attoxml-${ATTO_VERSION}/
        COMMAND tar -zcf attoxml-${ATTO_VERSION}.tgz
                attoxml-${ATTO_VERSION}
        COMMAND rm -rf attoxml-${ATTO_VERSION}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ELSE(UNIX)
    ADD_CUSTOM_TARGET(dist
        COMMAND ${CMAKE_COMMAND} -E tar zcf attoxml-${ATTO_VERSION}.tgz
                ${lib_hdrs} ${lib_srcs} ${EXTRA_DIST}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF(UNIX)
