#!/usr/bin/perl
# $Id: makedist.pl,v 1.1 2010/05/16 12:23:30 pafe Exp $

#  
# CONTACT     : patrick@pfp.de 
# AUTHOR      : Patrick Fedick <patrick@pfp.de> 
# 
# CREATED     : 2009/05/30 16:56:00 
# REVISION    : $Revision: 1.1 $  
# DATE        : $Date: 2010/05/16 12:23:30 $ 
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
rm -rf tmp.tar $DISTNAME
cd $PWD
rm -rf tmp
`;
print "\n";