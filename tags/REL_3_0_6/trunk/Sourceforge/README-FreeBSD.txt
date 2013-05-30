To install this Port on FreeBSD, you need at least FreeBSD Version 8.0.

To install this port, download the *.shar-File and put it somewhere on your
FreeBSD-Host. Then call

   sh WinMusik-*-FreeBSD-Port.shar

(replace * with the downloaded version)

This will unpack the shar-File and create a subdirectory called "winmusik".
This is actually like a "real" FreeBSD-Port from the ports collection, so
you can call "make install", "make package" or "make deinstall" inside the
subdirectory, like you do with other ports:

   su root
   cd winmusik
   make install
   
All Dependencies will be compiled and installed automatically. If you want
to make a binary package on your system, type "make package".

To deinstall WinMusik, call "pkg_delete WinMusik-3.0.6" (check for the
correct version) or call "make deinstall" inside the port directory.