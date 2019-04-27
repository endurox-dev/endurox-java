#!/usr/bin/perl
##
## @brief @(#) Generate wrapper code for Enduro/X C side functions
##  this parses header files, exports the wrapper header and
##  produces the dynamic loader and function lookup sources for
##  the loader library.
##
## @file genwrap.pl
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



@ARGS = '-' if !@ARGV;

#
# Read all CLI files..
#
for my $qfn (@ARGV) {
	open($fh, $qfn);

	while (<$fh>) {
		chomp;
		my $line = $_;
		my $func = "";
		my $typ = "";
		my $sign = "";
		
		
		printf( "got: [$line]\n");
		
		if ($line ~= /^JNIEXPORT/)
		{
			($typ, $func) = $line =~ /^JNIEXPORT (.*) JNICALL (.*)/g;
			my $line2 = <$fh>;
			chomp $line2;
			
			printf("got line 2 [$line2]\n");
			
		}
	}
	close($fh);
}







