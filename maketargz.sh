#!/bin/sh

PACKAGE=lua-gd
VERSION=alpha-2
DIRNAME=$PACKAGE-$VERSION
TGZNAME=$DIRNAME.tar.gz

rm -f $TGZNAME
mkdir $DIRNAME

cp -r COPYING $DIRNAME
cp -r Makefile $DIRNAME
cp -r README $DIRNAME
cp -r luagd.c $DIRNAME
cp -r demos $DIRNAME
rm -rf $DIRNAME/demos/CVS
rm -f $DIRNAME/demos/out.png
cp -r doc $DIRNAME
rm -rf $DIRNAME/doc/CVS

tar -czf $TGZNAME $DIRNAME
rm -rf $DIRNAME
tar -tzf $TGZNAME
