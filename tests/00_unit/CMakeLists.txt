##
## @brief Unit test for Enduro/X java
##
## @file CMakeLists.txt
##
## -----------------------------------------------------------------------------
## Enduro/X Middleware Platform for Distributed Transaction Processing
## Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
## Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
## This software is released under one of the following licenses:
## LGPL or Mavimax's license for commercial use.
## See LICENSE file for full text.
##
## C (as designed by Dennis Ritchie and later authors) language code is licensed
## under Enduro/X Modified GNU Affero General Public License, version 3.
## See LICENSE_C file for full text.
## -----------------------------------------------------------------------------
## LGPL license:
## 
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU Lesser General Public License, version 3 as published
## by the Free Software Foundation;
##
## This program is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
## PARTICULAR PURPOSE. See the GNU Lesser General Public License, version 3
## for more details.
##
## You should have received a copy of the Lesser General Public License along 
## with this program; if not, write to the Free Software Foundation, Inc., 
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
##
## -----------------------------------------------------------------------------
## A commercial use license is available from Mavimax, Ltd
## contact@mavimax.com
## -----------------------------------------------------------------------------
##

# Set minimum version
cmake_minimum_required(VERSION 3.1)

find_package(Java)
include(UseJava)

# Add debug options
# By default if RELEASE_BUILD is not defined, then we run in debug!
IF ($ENV{RELEASE_BUILD})
	# do nothing
ELSE ($ENV{RELEASE_BUILD})
	ADD_DEFINITIONS("-D UBF_DEBUG")
ENDIF ($ENV{RELEASE_BUILD})

# Make sure the compiler can find include files from our UBF library.
include_directories (. ${JNI_INCLUDE_DIRS})

FILE(GLOB JAVASRC *.java)

ndrx_java_ubf(
            TARGET 
                ubf00
            SOURCES
                test.fd
                Exfields
)

add_jar(jexunit00
    SOURCES
        ${JAVASRC}
    INCLUDE_JARS 
        ${CMAKE_SOURCE_DIR}/libsrc/enduroxjava.jar 
        ${CMAKE_SOURCE_DIR}/tests/libs/hamcrest-core.jar 
        ${CMAKE_SOURCE_DIR}/tests/libs/junit.jar
        ubf00.jar
        )

#
# TODO: Might want to add function for the binary generator:
# https://stackoverflow.com/questions/33696412/cmake-add-custom-command-argument-based-on-variable-content
#

#add_custom_command(OUTPUT jexunit00b
#               COMMAND ${CMAKE_BINARY_DIR}/exjld/exjld
#                        -o jexunit00b
#                        -m org.junit.runner.JUnitCore
#                        -I ${CMAKE_SOURCE_DIR}/include
#                        -L ${CMAKE_BINARY_DIR}/libexjlds 
#                        -L ${NDRX_JAVA_LIBDIR_JAVA} 
#                        -L ${NDRX_JAVA_LIBDIR_JVM}
#                        jexunit00.jar 
#                        ${CMAKE_SOURCE_DIR}/libsrc/enduroxjava.jar  
#                        ${CMAKE_SOURCE_DIR}/tests/libs/hamcrest-core.jar  
#                        ${CMAKE_SOURCE_DIR}/tests/libs/junit.jar
#               DEPENDS jexunit00.jar)

#add_custom_target( jexunit00bb ALL DEPENDS jexunit00b )

#
# Also we want to generate so UBF tables
#

ndrx_java_link(
            TARGET 
                jexunit00b
            MAINCLASS 
                org.junit.runner.JUnitCore
            JARS
                jexunit00.jar
                ${CMAKE_SOURCE_DIR}/libsrc/enduroxjava.jar  
                ${CMAKE_SOURCE_DIR}/tests/libs/hamcrest-core.jar  
                ${CMAKE_SOURCE_DIR}/tests/libs/junit.jar
                ubf00.jar
)
# vim: set ts=4 sw=4 et smartindent:
