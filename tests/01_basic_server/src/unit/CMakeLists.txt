##
## @brief Test01 Unit tests
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

add_jar(jexunit01
    SOURCES
        ${JAVASRC}
    INCLUDE_JARS 
        ${CMAKE_SOURCE_DIR}/libsrc/enduroxjava.jar 
        ${CMAKE_SOURCE_DIR}/tests/libs/hamcrest-core.jar 
        ${CMAKE_SOURCE_DIR}/tests/libs/junit.jar
        ../ubftab/ubf01.jar
        )

ndrx_java_link(
	TARGET 
            jexunit01b
	MAINCLASS 
            org.junit.runner.JUnitCore
	JARS
            jexunit01.jar
            ${CMAKE_SOURCE_DIR}/libsrc/enduroxjava.jar  
            ${CMAKE_SOURCE_DIR}/tests/libs/hamcrest-core.jar  
            ${CMAKE_SOURCE_DIR}/tests/libs/junit.jar
            ../ubftab/ubf01.jar
)

# vim: set ts=4 sw=4 et smartindent:
