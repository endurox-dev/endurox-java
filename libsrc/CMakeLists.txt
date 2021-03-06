##
## @brief Enduro/X JNI Backing library
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

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()


# Add debug options
# By default if RELEASE_BUILD is not defined, then we run in debug!
IF ($ENV{RELEASE_BUILD})
	# do nothing
ELSE ($ENV{RELEASE_BUILD})
	ADD_DEFINITIONS("-D UBF_DEBUG")
ENDIF ($ENV{RELEASE_BUILD})

find_package(JNI)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

# Make sure the compiler can find include files from our UBF library.
include_directories (. ${JNI_INCLUDE_DIRS})

FILE(GLOB JAVASRC java/org/endurox/*.java)
FILE(GLOB JAVASRC_EXCEPTIONS java/org/endurox/exceptions/*.java)

if(Java_VERSION VERSION_EQUAL 1.10 OR Java_VERSION VERSION_GREATER 1.10 )
    message (STATUS "Java Version >=10")
    set(CMAKE_JAVA_COMPILE_FLAGS -h ${PROJECT_SOURCE_DIR}/include)
endif()

add_jar(enduroxjava
        ${JAVASRC}
        ${JAVASRC_EXCEPTIONS}
        )

if(Java_VERSION VERSION_LESS 1.10)
    # Create JNI headers
    # this is superseeded in new version with compilers -h argument where to plot the headers.
    create_javah(TARGET enduroxjh CLASSES
        org.endurox.AtmiCtx
        org.endurox.TypedBuffer
        org.endurox.TypedUbf
        org.endurox.TypedString
        org.endurox.TypedJson
        org.endurox.TypedCarray
        org.endurox.TypedView
        org.endurox.TpInit
        org.endurox.BFldLocInfo
        org.endurox.BExprTree
        org.endurox.TpgetrplyResult
        org.endurox.TpTypesResult
        org.endurox.UnsolCallback
        org.endurox.TPQCTL
        org.endurox.TPTRANID
        java.lang.Runnable
    OUTPUT_DIR ${PROJECT_SOURCE_DIR}/include
    DEPENDS enduroxjava
    CLASSPATH enduroxjava)
endif()

add_subdirectory (c)

#
# Install the jars to the system
#
install (FILES
    enduroxjava.jar
    DESTINATION share/java)

# vim: set ts=4 sw=4 et smartindent:
