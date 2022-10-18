#!/bin/bash
data_dir=data
files=(
    4K.dat
    8K.dat
    16K.dat
    32K.dat
    64K.dat
    128K.dat
    256K.dat
    512K.dat
    1M.dat
    2M.dat
    4M.dat
    8M.dat
    16M.dat
    32M.dat
    64M.dat
    128M.dat
    256M.dat
    512M.dat
    1G.dat
    2G.dat
    4G.dat
    8G.dat
)

for file in ${files[@]}; do
    size=$(echo $file | cut -d. -f1)
    dd if=/dev/zero of=$data_dir/$file bs=$size count=0 seek=1 &> /dev/null
done

exit 0