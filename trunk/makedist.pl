#!/usr/bin/perl
#
# This file is part of WinMusik 3 by Patrick Fedick
#
# $Author: pafe $
# $Revision: 1.2 $
# $Date: 2010/05/16 12:40:40 $
# $Id: makedist.pl,v 1.2 2010/05/16 12:40:40 pafe Exp $
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


my $CVSTREE="WinMusik/wm30";
my $PACKAGE="WinMusik";
my $TAG=$ARGV[0];

if (!$TAG) {
	print "Aufruf: makedist.pl CVSTAG\n";
	exit;
}

$TAG =~ /^REL_(.*)$/;

my $VERSION=lc($1);
$VERSION =~ s/_/\./g;
if ($TAG eq "HEAD") {
	$VERSION="HEAD";
}
my $DISTNAME="$PACKAGE-$VERSION";

print "Packe $PACKAGE mit dem Tag \"$TAG\" aus dem CVS aus...\n";
print "Version:  $VERSION\n";
print "Distname: $DISTNAME\n";

my $PWD=`pwd`;
chomp($PWD);
my $err=`mkdir -p tmp
cd tmp
rm -rf $CVSTREE
cvs -d :ext:patrick\@cvs.pfp.de:/var/cvsroot checkout -r $TAG $CVSTREE 2>&1`;

if ($? != 0 ) {
	print "ERROR: Version konnte nicht aus dem CVS geholt werden!\n";
	print $err;
	print "\n";
	exit;
}

print "Aktualisiere Dokumentation...\n";
`doxygen Doxyfile`;
print " ok\n";
print "Kopiere Doku...\n";
`mkdir -p tmp/$CVSTREE/documentation; cd documentation; find html | cpio -pdmv ../tmp/$CVSTREE/documentation`;
print " done\n";

print "Erstelle Distribution fuer Version: $VERSION\n";
print "PWD=$PWD\n";
print `mkdir -p distfiles
rm -f distfiles/$DISTNAME*
mkdir -p distfiles/$DISTNAME
cd tmp/$CVSTREE
tar -cf $PWD/distfiles/tmp.tar --exclude *.core --exclude CVS --exclude .cvsignore resources src docs forms documentation include *.TXT Doxyfile *.qm *.rc *.qrc *.pro *.ts
cd $PWD
cd distfiles/$DISTNAME
tar -xf ../tmp.tar
cd ..
tar -cjf $DISTNAME.tar.bz2 $DISTNAME
cp $DISTNAME/docs/Userguide_de.pdf $DISTNAME-Userguide_de.pdf
rm -rf tmp.tar $DISTNAME
cd $PWD
rm -rf tmp
`;
print "\n";