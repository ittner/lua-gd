#!/bin/sh

PACKAGE=lua-gd
VERSION=2.0.33r2

DIRNAME=$PACKAGE-$VERSION
TGZNAME=$DIRNAME.tar.gz

rm -f $TGZNAME
mkdir $DIRNAME

cp -r COPYING $DIRNAME
cp -r Makefile $DIRNAME
cp -r README $DIRNAME
cp -r luagd.c $DIRNAME
cp -r lua-gd.spec $DIRNAME
cp -r test_features.lua $DIRNAME

mkdir $DIRNAME/debian
cp debian/changelog debian/control debian/copyright debian/rules \
    $DIRNAME/debian

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
