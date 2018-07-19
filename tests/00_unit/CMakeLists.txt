## 
## Unit test for Enduro/X java
##
## @file CMakeLists.txt
## 
## -----------------------------------------------------------------------------
## Enduro/X Middleware Platform for Distributed Transaction Processing
## Copyright (C) 2018, Mavimax, Ltd. All Rights Reserved.
## This software is released under one of the following licenses:
## GPL or Mavimax's license for commercial use.
## -----------------------------------------------------------------------------
## GPL license:
## 
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation; either version 2 of the License, or (at your option) any later
## version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
## PARTICULAR PURPOSE. See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, write to the Free Software Foundation, Inc., 59 Temple
## Place, Suite 330, Boston, MA 02111-1307 USA
##
## -----------------------------------------------------------------------------
## A commercial use license is available from Mavimax, Ltd
## contact@mavimax.com
## -----------------------------------------------------------------------------
##

# Set minimum version
cmake_minimum_required(VERSION 2.6)

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

add_jar(jexunit00
    SOURCES
        ${JAVASRC}
    INCLUDE_JARS 
        ${CMAKE_SOURCE_DIR}/libsrc/enduroxjava.jar 
        ${CMAKE_SOURCE_DIR}/tests/libs/hamcrest-core.jar 
        ${CMAKE_SOURCE_DIR}/tests/libs/junit.jar
        )

#
# TODO: Might want to add function for the binary generator:
# https://stackoverflow.com/questions/33696412/cmake-add-custom-command-argument-based-on-variable-content
#

add_custom_command(OUTPUT jexunit00_cmd
               COMMAND ${CMAKE_BINARY_DIR}/exjld/exjld
                        -o jexunit00b
                        -m org.junit.runner.JUnitCore
                        -I ${CMAKE_SOURCE_DIR}/include
                        -L ${CMAKE_BINARY_DIR}/libexjlds 
                        -L ${NDRX_JAVA_LIBDIR_JAVA} 
                        -L ${NDRX_JAVA_LIBDIR_JVM}
                        jexunit00.jar 
                        ${CMAKE_SOURCE_DIR}/libsrc/enduroxjava.jar  
                        ${CMAKE_SOURCE_DIR}/tests/libs/hamcrest-core.jar  
                        ${CMAKE_SOURCE_DIR}/tests/libs/junit.jar
               DEPENDS jexunit00.jar)


add_custom_target( jexunit00b ALL DEPENDS jexunit00_cmd )

