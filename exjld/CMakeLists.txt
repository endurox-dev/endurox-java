## 
## Enduro/X Java Linker
##
## @file CMakeLists.txt
## 
## -----------------------------------------------------------------------------
## Enduro/X Middleware Platform for Distributed Transaction Processing
## Copyright (C) 2015, Mavimax, Ltd. All Rights Reserved.
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

# Set a minimum version
cmake_minimum_required(VERSION 2.8)

# Make sure the compiler can find include files from UBF library
include_directories ( ${PROJECT_SOURCE_DIR}/include)

# Exectuables
add_executable (exjld   exjld.c
                        findclasses.c
                        resources.c
                        emb.c
                        codegen.c
                )

# Link the executable to the UBF library & others...
target_link_libraries (exjld atmiclt atmi ubf m nstd ${RT_LIB} pthread)

install (TARGETS 
    exjld
    DESTINATION bin)
