#!/bin/sh

PACKAGE=lua-gd
VERSION=beta-1

DIRNAME=$PACKAGE-$VERSION
TGZNAME=$DIRNAME.tar.gz

rm -f $TGZNAME
mkdir $DIRNAME

cp -r COPYING $DIRNAME
cp -r Makefile $DIRNAME
cp -r README $DIRNAME
cp -r luagd.c $DIRNAME
cp -r test_features.lua $DIRNAME

cp -r demos $DIRNAME
rm -rf $DIRNAME/demos/CVS
rm -rf $DIRNAME/demos/.xvpics
rm -f $DIRNAME/demos/out.*

cp -r doc $DIRNAME
rm -rf $DIRNAME/doc/CVS
rm -rf $DIRNAME/doc/.xvpics


tar -czf $TGZNAME $DIRNAME
rm -rf $DIRNAME
tar -tzf $TGZNAME
