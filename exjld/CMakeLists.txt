##
## @brief Enduro/X Java Linker
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

# Set a minimum version
cmake_minimum_required(VERSION 3.1)

# Make sure the compiler can find include files from UBF library
include_directories ( ${PROJECT_SOURCE_DIR}/include)

# Exectuables
add_executable (exjld   exjld.c
                        findclasses.c
                        resources.c
                        codegen.c
                        zip.c
                )

# Link the executable to the UBF library & others...
target_link_libraries (exjld atmiclt atmi ubf nstd ${RT_LIB} m pthread)

install (TARGETS 
    exjld
    DESTINATION bin)
# vim: set ts=4 sw=4 et smartindent:
