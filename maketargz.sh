#!/bin/sh

PACKAGE=lua-gd
VERSION=alpha-1
FILES=COPYING README luagd.c bugs.jpg Makefile status.txt \
      *.lua tolkien.ttf

DIRNAME=$PACKAGE-$VERSION
TGZNAME=$DIRNAME.tar.gz
rm -f $TGZNAME
mkdir $DIRNAME
cp -r $FILES $DIRNAME
tar -czf $TGZNAME $DIRNAME
rm -rf $DIRNAME
