WinMusik

WinMusik is a program for archiving music titles from audio CDs, DVDs,
audio tapes, vinyl records, harddisks and other audio devices. It has a
smart editor and an easy to use search interface.

  - smart graphical user interface for editing your music titles
  - import of MP3 files on harddisk
  - ID3v2-Tag-support
  - supports cover images in MP3 files
  - extensive search functions
  - print cd cover
  - search lists
  - open source under the GNU GENERAL PUBLIC LICENSE v3
  - runs on Windows XP/Vista/7, Linux, FreeBSD and other
    unixes

WWW: http://www.winmusik.de/
Author: Patrick Fedick <patrick@pfp.de>, http://www.pfp.de/


WinMusik 3 - Installation
=========================

This installation guide describes how to compile WinMusik on a Unix
compatible operation system from sourcecode. If that's not what you want,
please check https://sourceforge.net/projects/winmusik/files/client/
if there is a precompiled package for your operating system. 


Prerequirements
===============

You have to install the following packages:
	- C++ compiler with c++11 support (gcc 4.8, clang4 or higher)
	- QT, version 5.x or higher
	- zlib (developer package)
	- bzip2 (developer package)
	- pcre (developer package)
	- openssl (developer package)
	- libcurl (developer package)
	- iconv (only if you have to import files from old WinMusik version 2.x)
	- libcdio (developer package)


Configure and compile
=====================

WinMusik comes with an Autoconf configure-Skript. In a perfect world, you
can simply type

	./configure

and the script will finds everything automatically. In practice, it probably
will not, and you have to specify some paths manually. See 

	./configure --help
	
for a description of all parameters.


Then type

	qmake
	make release

You should find a WinMusik binary in the release-Folder