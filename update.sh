#!/bin/sh
./merge_pplib.sh
./genConfigure
sh conf.sh
make clean
make -j6 release

