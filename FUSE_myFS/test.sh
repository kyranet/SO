#!/bin/bash

MPOINT="./mount-point"

rm -R -f temp
mkdir temp

cp ./src/fuseLib.c ./temp/fuseLib.c
cp ./src/fuseLib.c $MPOINT/fuseLib.c
echo "Copying fuseLib.c"
cp ./src/myFS.h ./temp/myFS.h
cp ./src/myFS.h $MPOINT/myFS.h
echo "Copying myFS.h"

read -p "Press enter..."

ls -la $MPOINT
././my-fsck ./virtual-disk
diff -y ./src/fuseLib.c $MPOINT/fuseLib.c
diff -y ./src/myFS.h $MPOINT/myFS.h

truncate -c -o -s -1 ./temp/fuseLib.c
truncate -c -o -s -1 $MPOINT/fuseLib.c
echo "fuseLib.c truncated"
read -p "Press enter..."

ls -la $MPOINT
././my-fsck ./virtual-disk
diff -y ./src/fuseLib.c $MPOINT/fuseLib.c
read -p "Press enter..."

cp ./src/MyFileSystem.c ./temp/MyFileSystem.c
echo "Copying MyFileSystem.c"
cp ./temp/MyFileSystem.c $MPOINT/MyFileSystem.c
read -p "Press enter..."

ls -la $MPOINT
././my-fsck ./virtual-disk
diff -y ./src/MyFileSystem.c $MPOINT/MyFileSystem.c
read -p "Press enter..."

truncate -c -o -s +2 ./temp/myFS.h
truncate -c -o -s +2 $MPOINT/myFS.h
echo "myFS.h truncated"
read -p "Press enter..."

ls -la $MPOINT
././my-fsck ./virtual-disk
diff -y ./src/myFS.h $MPOINT/myFS.h
read -p "Press enter..."

echo "Making link fuseLib.lnk to fuseLib.c"
ln -s /fuseLib.c $MPOINT/fuseLib.lnk
ls -la $MPOINT
././my-fsck ./virtual-disk
read -p "Press enter..."

diff -y $MPOINT/fuseLib.lnk $MPOINT/fuseLib.c
read -p "Press enter..."
