#!/bin/sh

PACKAGE=lua-gd
VERSION=alpha-1
FILES="COPYING README luagd.c Makefile status.txt *.lua lua-logo* bugs.jpg \
 tolkien.ttf lua-gd.png"

DIRNAME=$PACKAGE-$VERSION
TGZNAME=$DIRNAME.tar.gz
rm -f $TGZNAME
mkdir $DIRNAME
cp -r $FILES $DIRNAME
tar -czf $TGZNAME $DIRNAME
rm -rf $DIRNAME
tar -tzf $TGZNAME
