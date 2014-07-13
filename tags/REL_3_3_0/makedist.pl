#!/usr/bin/perl
#
# This file is part of WinMusik 3 by Patrick Fedick
#
# $Author: pafe $
# $Revision: 1.3 $
# $Date: 2010/10/16 14:25:09 $
# $Id: makedist.pl,v 1.3 2010/10/16 14:25:09 pafe Exp $
#
#
# Copyright (c) 2010 Patrick Fedick
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


my $PACKAGE="WinMusik";
my $TAG=$ARGV[0];

if (!$TAG) {
	print "Aufruf: makedist.pl CVSTAG\n";
	exit;
}

if (($TAG =~ /^SNAP_(.*)$/)) {
	my $DATE=$1;
	$VERSION=$ARGV[1];
	if (!$VERSION) {
		print "Aufruf: makedist.pl SNAP_yyyymmdd VERSION\n";
		print ("Bei einem Snapshot muss als zweiter Parameter die Version angegegen werden.");
		exit;		
	}
	$VERSION.="-SNAP";
	$VERSION.=$DATE;
} elsif ($TAG eq "HEAD") {
	$VERSION="HEAD";
	$TAG="trunk";
} else {
	$TAG =~ /^REL_(.*)$/;
	$VERSION=lc($1);
	$VERSION =~ s/_/\./g;
	$TAG="tags/$TAG";
}


my $DISTNAME="$PACKAGE-$VERSION";

print "Packe $PACKAGE mit dem Tag \"$TAG\" aus dem SVN aus...\n";
print "Version:  $VERSION\n";
print "Distname: $DISTNAME\n";

my $PWD=`pwd`;
chomp($PWD);
my $err=`mkdir -p tmp
cd tmp
svn co svn://server.pfp.de/winmusik/client/$TAG . 2>&1`;



if ($? != 0 ) {
	print "ERROR: Version konnte nicht aus dem SVN geholt werden!\n";
	print $err;
	print "\n";
	exit;
}

print "Aktualisiere Dokumentation...\n";
`cd tmp; doxygen Doxyfile`;
print " ok\n";

print "Erstelle Distribution fuer Version: $VERSION\n";
print "PWD=$PWD\n";
print `mkdir -p distfiles
rm -f distfiles/$DISTNAME*
mkdir -p distfiles/$DISTNAME
cd tmp
tar -cf $PWD/distfiles/tmp.tar --exclude *.core --exclude .svn resources src docs forms documentation include *.TXT Doxyfile *.qm *.rc *.qrc *.pro *.ts
cd $PWD
cd distfiles/$DISTNAME
tar -xf ../tmp.tar
cd ..
tar -cjf $DISTNAME.tar.bz2 $DISTNAME
cp $DISTNAME/docs/Userguide_de.pdf $DISTNAME-Userguide_de.pdf
rm -rf tmp.tar $DISTNAME
cd $PWD
#rm -rf tmp
`;
print "\n";