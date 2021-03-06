WinMusik
========

WinMusik is a program for archiving music titles from audio CDs, DVDs,
audio tapes, vinyl records, harddisks and other audio devices. It has a
smart editor and an easy to use search interface.

  - smart graphical user interface for editing your music titles
  - import of MP3 files on harddisk
  - ID3v2-Tag-support including cover images, key and energy
  - extensive search functions
  - print cd cover
  - search lists
  - playlists
  - "DJ-Mode" in playlists with support for Native Instrument's Traktor 3
    Cue-Points
  - open source under the GNU GENERAL PUBLIC LICENSE v3
  - runs on Windows Vista/7/8/10, Linux, FreeBSD and other
    unixes

WWW: http://www.winmusik.de/
Author: Patrick Fedick <patrick@pfp.de>, http://www.pfp.de/


Installation
============

This installation guide describes how to compile WinMusik on a Unix
compatible operation system from sourcecode.


Prerequirements
---------------

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
---------------------

WinMusik comes with an Autoconf configure-Skript. In a perfect world, you
can simply type

	./configure

and the script will finds everything automatically. In practice, it probably
will not, and you have to specify some paths manually. See 

	./configure --help
	
for a description of all parameters. By default, the application will be
installed in below /usr/local (e.g. /usr/local/bin, /usr/local/share...).
If you want to install somewhere else, use "--prefix=PATH" with configure.


Then type

	make

You should find a WinMusik binary in the release-Folder


Type

	make install
	
to install it.

Credits
=======
Uses code from my library "PPLIB" (see: https://github.com/pfedick/pplib)
which is under a BSD 2-clause license.
