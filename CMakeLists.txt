##
## @brief Enduro/X Java Bindings
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

cmake_minimum_required (VERSION 3.5) 
project (ENDUROX-JAVA) 
set(VERSION "1.0.1")
set(PROJ_NAME "Enduro/X Java")
set(RELEASE "1")

find_package(Java)
include(UseJava)

# Output the project version
set(NDRX_JAVA_VERSION_STR "${PROJ_NAME} ${VERSION}")

################################################################################
# Check versions if defined, export build env
################################################################################

if(DEFINED ENV{NDRX_BLD_VERSION})
        message (STATUS "Have build flags")
        IF(NOT $ENV{NDRX_BLD_VERSION} MATCHES ${VERSION})
                message( FATAL_ERROR "Invalid versions: ENV: [$ENV{NDRX_BLD_VERSION}] Code: [${VERSION}]" )
        endif()
endif()

if(DEFINED ENV{NDRX_BLD_RELEASE})
    set(RELEASE $ENV{NDRX_BLD_RELEASE})
endif()

message("CMake RELEASE  = ${RELEASE}")

SET (NDRX_BLD_PRODUCT   $ENV{NDRX_BLD_PRODUCT})
SET (NDRX_BLD_SYSTEM    $ENV{NDRX_BLD_SYSTEM})
SET (NDRX_BLD_CONFIG    $ENV{NDRX_BLD_CONFIG})
SET (NDRX_BLD_VERSION   $ENV{NDRX_BLD_VERSION})
SET (NDRX_BLD_RELEASE   $ENV{NDRX_BLD_RELEASE})
SET (NDRX_BLD_TAG       $ENV{NDRX_BLD_TAG})
SET (NDRX_BLD_BRANCH    $ENV{NDRX_BLD_BRANCH})
SET (NDRX_BLD_FLAGS     $ENV{NDRX_BLD_FLAGS})

################################################################################
# OS Configuration
################################################################################

find_package(JNI)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
endif()

EXEC_PROGRAM(uname ARGS -a OUTPUT_VARIABLE _TMP_CMAKE_OS_NAME)
string(REGEX MATCH "^[a-zA-Z0-9-]*" _TMP_CMAKE_OS_NAME_EXTR ${_TMP_CMAKE_OS_NAME})
string(TOUPPER ${_TMP_CMAKE_OS_NAME_EXTR} CMAKE_OS_NAME)

message("JAVA_HOME  = $ENV{JAVA_HOME}")
message("CMake CMAKE_OS_NAME  = ${CMAKE_OS_NAME}")
message("CMake CMAKE_C_COMPILER_ID  = ${CMAKE_C_COMPILER_ID}")
message("CMake CMAKE_CXX_COMPILER_ID  = ${CMAKE_CXX_COMPILER_ID}")


# Support #310 Ubuntu 18.04 prints lot of unneeded warnings...
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
#    SET(CMAKE_C_FLAGS "-Wno-format-truncation -Wstringop-overflow=0 -D_DEFAULT_SOURCE=1 ${CMAKE_C_FLAGS}")
    SET(CMAKE_C_FLAGS "-Wno-stringop-overflow -Wno-format-truncation -Wformat-overflow=0 ${CMAKE_C_FLAGS}")
endif()

# Enable release only if specified.
# By default if RELEASE_BUILD is not defined, then we run in debug!
IF (DEFINE_RELEASEBUILD)
	MESSAGE( RELEASE_BUILD )
	SET(CMAKE_BUILD_TYPE release)
        SET(CMAKE_C_FLAGS "-fstack-protector -O2 ${CMAKE_C_FLAGS}")
ELSE ()
	MESSAGE( DEBUG_BUILD )
	SET(CMAKE_BUILD_TYPE debug)
# Memory debugging:
#       SET(CMAKE_C_FLAGS "-fsanitize=alignment -fsanitize=address -fno-omit-frame-pointer -O1 -ggdb ${CMAKE_C_FLAGS}")
        SET(CMAKE_C_FLAGS "-fstack-protector -O2 -ggdb ${CMAKE_C_FLAGS}")
ENDIF ()

SET(RT_LIB rt dl)
if( ${CMAKE_OS_NAME} STREQUAL "LINUX" )
        set(EX_OS_LINUX "1")
        set(NDRX_LD_LIBRARY_PATH "LD_LIBRARY_PATH")
elseif(${CMAKE_OS_NAME} STREQUAL "AIX")
# Configure the compiler, we support XLC too..

        set(NDRX_LD_LIBRARY_PATH "LD_LIBRARY_PATH")

	if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
            message("GNU compiler")
            set ( CMAKE_C_FLAGS "-D_SEM_SEMUN_UNDEFINED -D_THREAD_SAFE -pthread -maix64 ${CMAKE_C_FLAGS}")
	elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "XL")
            message("XL compiler")
            SET ( CMAKE_CXX_FLAGS "-O2 -qtls -q64 -b64 -qsuppress=1500-030" CACHE STRING "compile flags" FORCE)
            set ( CMAKE_C_FLAGS " -D_SEM_SEMUN_UNDEFINED -D_THREAD_SAFE ")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qwarn64 -qpic -bexpfull -b64")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -brtl -qtls -q64 -qlanglvl=extc99")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsuppress=1506-997 -qprocimported=setjmp -lbsd")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsuppress=1506-747")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsuppress=1506-742")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsuppress=1506-743")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsuppress=1506-744")
            set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -qsuppress=1506-745")

            if (NOT DEFINE_RELEASEBUILD)
                    message("XL compiler: with -g (debug symbols...)")
                    set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
            endif()
	endif()

elseif(${CMAKE_OS_NAME} STREQUAL "HP-UX")
elseif(${CMAKE_OS_NAME} STREQUAL "SUNOS")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -m64")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -m64")

    set(NDRX_LD_LIBRARY_PATH "LD_LIBRARY_PATH")
    # Bug #219 !!! _REENTRANT makes errno thread safe
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT")

    if ("${CMAKE_C_COMPILER_ID}" STREQUAL "SunPro")
        # set new config for SunPro
	SET(CMAKE_C_FLAGS "-D_REENTRANT -m64")
        message("SUN Studio compiler (building for 64bit) ")

        # Bug #219 !!! _REENTRANT makes errno thread safe
        # For GNU I guess 
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -errtags -fast -xc99=all -erroff=E_PTR_TO_VOID_IN_ARITHMETIC ")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -erroff=E_EMPTY_MBR_DECLARATION -erroff=E_NO_IMPLICIT_DECL_ALLOWED")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -erroff=E_EMPTY_DECLARATION -erroff=E_ARGUEMENT_MISMATCH -erroff=E_ZERO_OR_NEGATIVE_SUBSCRIPT")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -erroff=E_ARG_INCOMPATIBLE_WITH_ARG_L")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -erroff=E_BAD_PTR_INT_COMBINATION")

        # Add Sun Studio C runtime
        SET(RT_LIB ${RT_LIB} Crun)
    endif()

elseif(${CMAKE_OS_NAME} STREQUAL "FREEBSD")
    SET(RT_LIB rt)
    set(NDRX_LD_LIBRARY_PATH "LD_LIBRARY_PATH")
elseif(${CMAKE_OS_NAME} STREQUAL "CYGWIN")
    set(NDRX_LD_LIBRARY_PATH "LD_LIBRARY_PATH")
elseif(${CMAKE_OS_NAME} STREQUAL "DARWIN")
    SET(RT_LIB dl)
    set(NDRX_LD_LIBRARY_PATH "DYLD_FALLBACK_LIBRARY_PATH")
# Unknown OS:
else()
	message( FATAL_ERROR "Unsupported OS" )
endif()


# JAVA_AWT_LIBRARY=/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/libjawt.so
# JAVA_JVM_LIBRARY=/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/libjvm.so

get_filename_component(NDRX_JAVA_LIBDIR_JAVA ${JAVA_AWT_LIBRARY} DIRECTORY)
get_filename_component(NDRX_JAVA_LIBDIR_JVM ${JAVA_JVM_LIBRARY} DIRECTORY)


################################################################################
# "Configure"
################################################################################

# In this file we are doing all of our 'configure' checks. Things like checking
# for headers, functions, libraries, types and size of types.
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckCSourceCompiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckCXXSourceCompiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)

# To check the size of a primitive type:
CHECK_TYPE_SIZE("int"   EX_SIZEOF_INT)
CHECK_TYPE_SIZE("long"  EX_SIZEOF_LONG)
CHECK_TYPE_SIZE("void*"  EX_SIZEOF_VOIDPTR)

MATH (EXPR EX_PLATFORM_BITS "${EX_SIZEOF_VOIDPTR} * 8")
################################################################################
# Install dir config
################################################################################
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)

message("LIB: ${LIB64}")

if(${CMAKE_OS_NAME} STREQUAL "DARWIN")
    set(LIB_SUFFIX "")
# Seems like BSD does not use lib64 folder
elseif ("${CMAKE_OS_NAME}" STREQUAL "FREEBSD")
    set(LIB_SUFFIX "")
elseif ("${CMAKE_OS_NAME}" STREQUAL "AIX")
    set(LIB_SUFFIX "")
elseif ("${LIB64}" STREQUAL "TRUE")
    set(LIB_SUFFIX 64)
else()
    set(LIB_SUFFIX "")
endif()

message("LIB_SUFFIX: ${LIB_SUFFIX}")

set(INSTALL_LIB_DIR lib${LIB_SUFFIX} CACHE PATH "Installation directory for libraries")

message("INSTALL_LIB_DIR: ${INSTALL_LIB_DIR}")

mark_as_advanced(INSTALL_LIB_DIR) 
MESSAGE( STATUS "INSTALL_LIB_DIR: " ${INSTALL_LIB_DIR} )

################################################################################
# Options
################################################################################
###############################################################################

################################################################################
# Option to disable documentation build (dos are enabled by default)
OPTION(DEFINE_DISABLEDOC "Disable documentation generation" OFF) 
################################################################################

################################################################################
# Functions
################################################################################

include(CMakeParseArguments)

# Call The Enduro/X Java linker
# @param TARGET output binary name
# @param MAINCLASS main class name
# @param JARS... List of jar files to link
function(ndrx_java_link)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET;MAINCLASS;" # list of names of mono-valued arguments
        "JARS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
    # note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
    if(NOT PARSED_ARGS_TARGET)
        message(FATAL_ERROR "You must provide target name")
    endif(NOT PARSED_ARGS_TARGET)

    if(NOT PARSED_ARGS_MAINCLASS)
        message(FATAL_ERROR "You must provide main class name")
    endif(NOT PARSED_ARGS_MAINCLASS)

    if(NOT PARSED_ARGS_JARS)
        message(FATAL_ERROR "You must provide list of jars to link")
    endif(NOT PARSED_ARGS_JARS)

    #set(JARS_LIST " ")
    message("Linking jars:")
    foreach(src ${PARSED_ARGS_JARS})
        message("- ${src}")
    endforeach(src)

    #set(DEPENDS_LIST " ")
    message("Depends:")
    foreach(src ${PARSED_ARGS_DEPENDS})
        message("- ${src}")
    endforeach(src)

    # We automatically depend on jars included
    add_custom_command(OUTPUT ${PARSED_ARGS_TARGET}
                   COMMAND ${CMAKE_COMMAND} -E env CFLAGS=${CMAKE_C_FLAGS}
                            CC=${CMAKE_C_COMPILER}
                            ${CMAKE_BINARY_DIR}/exjld/exjld
                            -o ${PARSED_ARGS_TARGET}
                            -m ${PARSED_ARGS_MAINCLASS}
                            -I ${CMAKE_SOURCE_DIR}/include
                            -L ${CMAKE_BINARY_DIR}/libexjlds 
                            -L ${NDRX_JAVA_LIBDIR_JAVA} 
                            -L ${NDRX_JAVA_LIBDIR_JVM}
                            -n
                            ${PARSED_ARGS_JARS}
                   DEPENDS ${PARSED_ARGS_JARS})

    add_custom_target( ${PARSED_ARGS_TARGET}link ALL DEPENDS ${PARSED_ARGS_TARGET} )
endfunction(ndrx_java_link)

# Generate Java UBF headers
# @param FILES comma seperated of source files to include include in build
# @param MAINCLASS main class name
# @param JARS... List of jar files to link
function(ndrx_java_ubf)
    cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET;PACKAGE" # list of names of mono-valued arguments
        "SOURCES" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
    # note: if it remains unparsed arguments, here, they can be found in variable PARSED_ARGS_UNPARSED_ARGUMENTS
    if(NOT PARSED_ARGS_TARGET)
        message(FATAL_ERROR "You must provide target name")
    endif(NOT PARSED_ARGS_TARGET)

    #if(NOT PARSED_ARGS_PACKAGE)
    #    message(FATAL_ERROR "You must provide package name")
    #endif(NOT PARSED_ARGS_PACKAGE)

    if(NOT PARSED_ARGS_SOURCES)
        message(FATAL_ERROR "You must provide list of fd files to compile")
    endif(NOT PARSED_ARGS_SOURCES)

    message("Generating FDs:")

    # add .java to the files
    # PARSED_ARGS_FILES
    SET(JAVA_LIST "")
    SET(FD_LIST "")
    FOREACH(f ${PARSED_ARGS_SOURCES})
        message("- ${f}")

        #string(REPLACE ".fd" ".java" JAVA_NAME "${f}")
        # strip off extension...
        string(REGEX REPLACE "\\..*$" "" JAVA_NAME "${f}")
        
        set(JAVA_NAME "${JAVA_NAME}.java")

        message("- Java name: ${JAVA_NAME}")

        LIST(APPEND JAVA_LIST "${JAVA_NAME}")
        set(FD_LIST "${FD_LIST},${f}")
    ENDFOREACH(f)

    # fix the fd list starting comma
    string(REGEX REPLACE "^," "" FD_LIST "${FD_LIST}")
    message("FDLIST=${FD_LIST}")

    # We automatically depend on jars included
    add_custom_command(OUTPUT ${JAVA_LIST}
                   COMMAND ${CMAKE_COMMAND} -E env FIELDTBLS=${FD_LIST}
                            FLDTBLDIR=${CMAKE_CURRENT_SOURCE_DIR}
                            #mkfldhdr -m2 -p ${PARSED_ARGS_PACKAGE} - will not use package
                            mkfldhdr -m2
                   DEPENDS ${PARSED_ARGS_SOURCES})

    add_custom_target( ${PARSED_ARGS_TARGET}gen ALL DEPENDS  )
    
    add_jar(${PARSED_ARGS_TARGET}
            SOURCES ${JAVA_LIST}
            )

endfunction(ndrx_java_ubf)

################################################################################
# Submakes
################################################################################
# Recurse into the project subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_subdirectory (libsrc)
add_subdirectory (include)
add_subdirectory (libexjlds)
add_subdirectory (exjld)
add_subdirectory (xadrv)
add_subdirectory (tests)

IF(DEFINE_DISABLEDOC)
        message("Documentation disabled - not building")
ELSE (DEFINE_DISABLEDOC)
        message("Building doc")
        add_subdirectory (doc) #  Documentation project.
ENDIF (DEFINE_DISABLEDOC)

################################################################################
# uninstall target
################################################################################
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
################################################################################
# Packages
################################################################################

find_program(LSB_RELEASE_EXECUTABLE lsb_release)
if(LSB_RELEASE_EXECUTABLE)
        execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -si
                OUTPUT_VARIABLE _TMP_LSB_RELEASE_OUTPUT_OS
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)

        string(TOLOWER 
                ${_TMP_LSB_RELEASE_OUTPUT_OS}
        LSB_RELEASE_OUTPUT_OS)
        string(REPLACE " " "_" LSB_RELEASE_OUTPUT_OS ${LSB_RELEASE_OUTPUT_OS})

        execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -sr
                OUTPUT_VARIABLE _TMP_LSB_RELEASE_OUTPUT_VER
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(REPLACE "." "_" LSB_RELEASE_OUTPUT_VER ${_TMP_LSB_RELEASE_OUTPUT_VER})
        #string(REGEX MATCH "^[0-9]+" LSB_RELEASE_OUTPUT_VER ${_TMP_LSB_RELEASE_OUTPUT_VER})
elseif (EXISTS /etc/os-release)

	execute_process(COMMAND bash -c "cat /etc/os-release | egrep '^NAME=' | cut -d '\"' -f2"
                OUTPUT_VARIABLE _TMP_LSB_RELEASE_OUTPUT_OS
                OUTPUT_STRIP_TRAILING_WHITESPACE)

        string(TOLOWER 
                ${_TMP_LSB_RELEASE_OUTPUT_OS}
        LSB_RELEASE_OUTPUT_OS)
        string(REPLACE " " "_" LSB_RELEASE_OUTPUT_OS ${LSB_RELEASE_OUTPUT_OS})

        # fixes for CentOs 7.1810, having something like "7 (Core)" in output.
        execute_process(COMMAND bash -c "cat /etc/os-release | egrep '^VERSION=' | cut -d ' ' -f1 | cut -d '\"' -f2"
                OUTPUT_VARIABLE _TMP_LSB_RELEASE_OUTPUT_VER
                OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(REPLACE "." "_" LSB_RELEASE_OUTPUT_VER ${_TMP_LSB_RELEASE_OUTPUT_VER})
        #string(REGEX MATCH "^[0-9]+" LSB_RELEASE_OUTPUT_VER ${_TMP_LSB_RELEASE_OUTPUT_VER})
else ()
        set(LSB_RELEASE_OUTPUT_OS ${CMAKE_OS_NAME})

        string(REPLACE "." "_" _TMP_LSB_RELEASE_OUTPUT_VER ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "-" "_" LSB_RELEASE_OUTPUT_VER ${_TMP_LSB_RELEASE_OUTPUT_VER})
	
	# If it is AIX, then we can extract version from uname
	if(${CMAKE_OS_NAME} STREQUAL "AIX")
		execute_process(COMMAND uname -v
			OUTPUT_VARIABLE _TMP_OS_MAJOR_VER OUTPUT_STRIP_TRAILING_WHITESPACE)
		execute_process(COMMAND uname -r
			OUTPUT_VARIABLE _TMP_OS_MINOR_VER OUTPUT_STRIP_TRAILING_WHITESPACE)
		set(LSB_RELEASE_OUTPUT_VER ${_TMP_OS_MAJOR_VER}_${_TMP_OS_MINOR_VER})
	endif()
	
endif()

message("LSB_RELEASE OS  = ${LSB_RELEASE_OUTPUT_OS}")
message("LSB_RELEASE VER = ${LSB_RELEASE_OUTPUT_VER}")
set(EX_LSB_RELEASE_VER ${LSB_RELEASE_OUTPUT_VER})

string(REPLACE "_" ";" LSB_VERSION_LIST ${LSB_RELEASE_OUTPUT_VER})

list( LENGTH LSB_VERSION_LIST TMP_LIST_LEN ) 

if (TMP_LIST_LEN LESS 1)
        # No version at all!!
        set(EX_LSB_RELEASE_VER_MAJOR "0")
        set(EX_LSB_RELEASE_VER_MINOR "0")
elseif (TMP_LIST_LEN LESS 2)
        # Bug #198 - FedoraCore do not have minor version numbers
        list(GET LSB_VERSION_LIST 0 EX_LSB_RELEASE_VER_MAJOR)
        set(EX_LSB_RELEASE_VER_MINOR "0")

else ()
        list(GET LSB_VERSION_LIST 0 EX_LSB_RELEASE_VER_MAJOR)
        list(GET LSB_VERSION_LIST 1 EX_LSB_RELEASE_VER_MINOR)
endif()

MESSAGE( "EX_LSB_RELEASE_VER_MAJOR = " ${EX_LSB_RELEASE_VER_MAJOR} )
MESSAGE( "EX_LSB_RELEASE_VER_MINOR = " ${EX_LSB_RELEASE_VER_MINOR} )

# Support only for Linux
MESSAGE( "CPack:Debug: CMAKE_SYSTEM_NAME = " ${CMAKE_SYSTEM_NAME} )
MESSAGE( "CPack:Debug: CMAKE_SYSTEM_PROCESSOR = " ${CMAKE_SYSTEM_PROCESSOR} )
set(CPACK_MONOLITHIC_INSTALL 1)

#
# Generate configuration 
#
configure_file ("${PROJECT_SOURCE_DIR}/include/ndrx_java_config.h.in"
                "${PROJECT_BINARY_DIR}/include/ndrx_java_config.h" )

# Test the available generators and then configure output
find_program(RPMPROG "rpmbuild")
find_program(APTPROG "dpkg")

# avoid file /usr/share/man from install of endurox-3.5.1-1.x86_64 conflicts with file from package filesystem-3.2-21.el7.x86_64
# problems...
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION /usr/share/man
		/usr/share/man/man3
		/usr/share/man/man5
		/usr/share/man/man8
		/usr/lib64/pkgconfig
		/usr/lib/pkgconfig)
message("Excl: ${CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION}")

set(CPACK_GENERATOR "TGZ")
if(RPMPROG)
        message("Outputting RPM")
	set(CPACK_GENERATOR "${CPACK_GENERATOR};RPM")
endif()

if(APTPROG)
        message("Outputting DEB")
	set(CPACK_GENERATOR "${CPACK_GENERATOR};DEB")
endif()

#
# Fix arch for AIX
#
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^powerpc")
	set(CPACK_RPM_PACKAGE_ARCHITECTURE "ppc")
ENDIF()

set(CPACK_PACKAGE_CONTACT "madars.vitolins@gmail.com")
set(CPACK_PACKAGE_VERSION ${VERSION})
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libxml2,openssl,libgpgme11")
#set(CPACK_RPM_PACKAGE_REQUIRES "libxml2,openssl,libssl,libgpgme11")
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LO)
message("CPack:Debug: PROJECT NAME = ${PROJECT_NAME_LO}")
set(CPACK_PACKAGE_FILE_NAME         ${PROJECT_NAME_LO}-${VERSION}-${RELEASE}.${LSB_RELEASE_OUTPUT_OS}${LSB_RELEASE_OUTPUT_VER}_java${Java_VERSION_MAJOR}_${Java_VERSION_MINOR}.${CMAKE_SYSTEM_PROCESSOR}_${EX_PLATFORM_BITS})
message("CPack:Debug: CPACK_PACKAGE_FILE_NAME = ${CPACK_PACKAGE_FILE_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Enduro/X Middleware Platform")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION  ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_PACKAGE_AUTOREQ "0")
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")

include(CPack)
################################################################################
# Source indexing for quick symbol lookup
# note that "glimpse" package must be installed.
# to exclude any directory or name from indexing,
# put the name in the glimpse_index/.glimpse_exclude
################################################################################

add_custom_target(index glimpseindex -n -H ./glimpse_index ${PROJECT_SOURCE_DIR}
                    COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/glimpse_index/glim.cmake)
#this also shall generate the glimp command in the current project

# vim: set ts=4 sw=4 et smartindent:
