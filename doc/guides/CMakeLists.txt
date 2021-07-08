##
## @brief Build documentation
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
cmake_minimum_required(VERSION 3.1)

find_program(A2X_EXECUTABLE NAMES a2x)

set(HTML_NAMES module_developer_guide getting_started_tutorial)

if (A2X_EXECUTABLE)

    execute_process(COMMAND a2x "--version" RESULT_VARIABLE RET)

    if( NOT ${RET} MATCHES "0")
        message("a2x does not work - disabling doc...")
        unset(A2X_EXECUTABLE)
    elseif()


    # Generate HTMLs
    set(A2X_OPTS_HTML -D . -f xhtml)
    set(HTML_FILES)
    foreach(m IN LISTS HTML_NAMES)
        set(mf ${CMAKE_CURRENT_BINARY_DIR}/${m}.html)
        set(ms ${CMAKE_CURRENT_SOURCE_DIR}/${m}.adoc)
        add_custom_command(OUTPUT ${mf}
            COMMAND ${A2X_EXECUTABLE} ${A2X_OPTS_HTML} ${ms}
            DEPENDS ${ms}
            WORKING_DIRECTORY .
            COMMENT "Building HTML ${mf}"
            VERBATIM)
        list(APPEND HTML_FILES ${mf})

        # Install the docs
        install (FILES
            ${mf}
            DESTINATION share/endurox-java/doc/html/guides)

    endforeach()
    add_custom_target(html ALL DEPENDS ${HTML_FILES})

    install (FILES
        ${CMAKE_CURRENT_BINARY_DIR}/docbook-xsl.css
        DESTINATION share/endurox-java/doc/html/guides
        OPTIONAL)
    endif()
# if A2X_EXECUTABLE
endif()
# end.

# vim: set ts=4 sw=4 et smartindent:
