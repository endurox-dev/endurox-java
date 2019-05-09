##
## @brief Enduro/X Java Linker static library (for building c bin)
##
## @file CMakeLists.txt
##
## -----------------------------------------------------------------------------
## Enduro/X Middleware Platform for Distributed Transaction Processing
## Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
## Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
## This software is released under one of the following licenses:
## AGPL or Mavimax's license for commercial use.
## -----------------------------------------------------------------------------
## AGPL license:
## 
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU Affero General Public License, version 3 as published
## by the Free Software Foundation;
##
## This program is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
## PARTICULAR PURPOSE. See the GNU Affero General Public License, version 3
## for more details.
##
## You should have received a copy of the GNU Affero General Public License along 
## with this program; if not, write to the Free Software Foundation, Inc., 
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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

find_package(JNI)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

# Make sure the compiler can find include files from our UBF library.
include_directories (. ${JNI_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

# Generate class of static loader
add_custom_command(
  OUTPUT StaticClassLoader.class BytesURLHandler.class BytesURLConnection.class
  COMMAND javac StaticClassLoader.java BytesURLHandler.java BytesURLConnection.java
  DEPENDS StaticClassLoader.java BytesURLHandler.java BytesURLConnection.java)


# Generate include for static loader
add_custom_command(
  OUTPUT StaticClassLoader.cinclude
  COMMAND exembedfile StaticClassLoader.class StaticClassLoader cinclude
  DEPENDS StaticClassLoader.class)

add_custom_command(
  OUTPUT BytesURLHandler.cinclude
  COMMAND exembedfile BytesURLHandler.class BytesURLHandler cinclude
  DEPENDS BytesURLHandler.class)

add_custom_command(
  OUTPUT BytesURLConnection.cinclude
  COMMAND exembedfile BytesURLConnection.class BytesURLConnection cinclude
  DEPENDS BytesURLConnection.class)

# Shared static loader library
add_library (exjlds SHARED
                exj.c
                cp.c
                except.c
                StaticClassLoader.cinclude
                BytesURLHandler.cinclude
                BytesURLConnection.cinclude
	)

# target_link_libraries (exjld atmisrvinteg atmi ubf nstd ${RT_LIB} pthread m)

install (TARGETS 
    exjlds
    DESTINATION "${INSTALL_LIB_DIR}"
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
    COMPONENT libraries
    )

# vim: set ts=4 sw=4 et smartindent:
