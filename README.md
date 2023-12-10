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
  - C++ compiler with c++11 support (gcc or clang)
  - QT, version 5.x or higher
  - zlib (developer package)
  - bzip2 (developer package)
  - pcre (developer package)
  - openssl (developer package)
  - libcurl (developer package)
  - iconv (only if you have to import files from old WinMusik version 2.x)
  - libcdio (developer package)

### Fedora 39

    sudo dnf install gcc gcc-c++ qt5-qtbase-devel libcdio-devel zlib-devel \
        bzip2-devel libpng-devel pcre-devel


### Ubuntu 22.04

    sudo apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools dpkg-dev \
        libcdio-dev zlib1g-dev libbz2-dev libpng-dev libpcre3-dev

### Windows 10 with Msys/mingw

For installing msys2 and mingw64, please see: [https://www.msys2.org/](https://www.msys2.org/)

    pacman -S git make mingw-w64-x86_64-gcc mingw-w64-x86_64-zlib mingw-w64-x86_64-libiconv \
         mingw-w64-x86_64-bzip2 \
         mingw-w64-x86_64-pcre mingw-w64-x86_64-libpng mingw-w64-x86_64-qt5

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
