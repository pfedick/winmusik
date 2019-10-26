#!/bin/sh
./merge_pplib.sh
./genConfigure
sh conf.sh
qmake
make -j6 release

