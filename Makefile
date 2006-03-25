# luagd -- d bindings for the Lua Programming Language.
# (c) 2004-06 Alexandre Erwin Ittner <aittner@netuno.com.br>
#
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#

CC=gcc

# ---------------------------------------------------------------------------
# Automatic configuration using pkgconfig, gd-config and sed. These
# lines should work on most Linux/Unix systems. If your system does not
# have these programs you must comment out these lines and uncomment and
# change the next ones.

# Name of .pc file. "lua5.1" on Debian/Ubuntu
LUAPKG=lua5.1
OUTFILE=gd.so
CFLAGS=`gdlib-config --cflags` `pkg-config $(LUAPKG) --cflags` -O3 -Wall
GDFEATURES=`gdlib-config --features |sed -e "s/GD_/-DGD_/g"`
LFLAGS=-shared `gdlib-config --ldflags` `gdlib-config --libs` \
    `pkg-config $(LUAPKG) --libs` -lgd
INSTALL_PATH=`pkg-config $(LUAPKG) --variable=INSTALL_CMOD`


# ---------------------------------------------------------------------------
# Manual configuration for systems without pkgconfig.

#CFLAGS=-Wall `gdlib-config --cflags` -O3
#GDFEATURES=`gdlib-config --features |sed -e "s/GD_/-DGD_/g"`
#LFLAGS=-shared `gdlib-config --ldflags` `gdlib-config --libs` -llua -lgd 
#INSTALL_PATH=/usr/lib/lua/


# ---------------------------------------------------------------------------
# Manual configuration for Windows and systems without sed, pkgconfig, etc.
# Uncomment, change and good luck :)

#OUTFILE=gd.dll
#CFLAGS=-Wall -O3
#GDFEATURES=-DGD_XPM -DGD_JPEG -DGD_FONTCONFIG -DGD_FREETYPE -DGD_PNG -DGD_GIF
#LFLAGS=-shared -lgd2 -lm -llua -llualib
#INSTALL_PATH="C:/Program Files/lua/"
# ---------------------------------------------------------------------------


all: $(OUTFILE)

$(OUTFILE): luagd.c
	$(CC) -o $(OUTFILE) $(GDFEATURES) $(CFLAGS) $(LFLAGS) luagd.c
	lua test_features.lua

install: $(OUTFILE)
	install -s $(OUTFILE).so $(INSTALL_PATH)

clean:
	rm -f $(OUTFILE) *.o
