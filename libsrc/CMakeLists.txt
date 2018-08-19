## 
## Enduro/X JNI Backing library
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

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()


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

add_jar(enduroxjava
        ${JAVASRC}
        ${JAVASRC_EXCEPTIONS}
        )

# Create JNI headers
create_javah(TARGET enduroxjh CLASSES
        org.endurox.AtmiCtx
        org.endurox.TypedBuffer
        org.endurox.TypedUBF
        org.endurox.TpInit
        org.endurox.BFldLocInfo
        org.endurox.BExprTree
    OUTPUT_DIR ${PROJECT_SOURCE_DIR}/include
    DEPENDS enduroxjava
    CLASSPATH enduroxjava)

# Create a library called "UBF" which includes the source files.
# The extension is already found. Any number of sources could be listed here. 
#add_library (exjava SHARED
#		AtmiCtx.c
#        AtmiBuf.c
#        exceptions.c
#	)


#add_dependencies(exjava enduroxjh)


add_subdirectory (c)

#    target_link_libraries(exjava atmisrvinteg atmi ubf nstd pthread)
#if(${CMAKE_OS_NAME} STREQUAL "CYGWIN")
#    target_link_libraries(exjava nstd)
#elseif(${CMAKE_OS_NAME} STREQUAL "DARWIN")
#    target_link_libraries(eoxjava nstd)
#elseif(${CMAKE_OS_NAME} STREQUAL "AIX")
#    # for XL tls
#    target_link_libraries(enduroxjava pthread)
#endif()

#install (TARGETS 
#    exjava
#    DESTINATION "${INSTALL_LIB_DIR}"
#    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
#    COMPONENT libraries
#    )

