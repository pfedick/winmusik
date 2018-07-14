#!/bin/sh
ldd release/WinMusik.exe | grep mingw | sort -u | while read ddl_name arrow filename rest
do
	#echo "$filename"
	echo "Source: \"C:/msys64/$filename\"; DestDir: \"{app}\"; Flags: ignoreversion;"
	
done