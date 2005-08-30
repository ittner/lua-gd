# luagd -- d bindings for the Lua Programming Language.
# (c) 2004-05 Alexandre Erwin Ittner <aittner@netuno.com.br>
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

# Automatic configuration using gdlib-config and sed. These lines should
# work on most Linux/Unix systems. If your system does not have these programs
# you must comment out these lines and uncomment and change the next ones.

OUTFILE=libluagd.so
CFLAGS=-Wall `gdlib-config --cflags` -O3
GDFEATURES=`gdlib-config --features |sed -e "s/GD_/-DGD_/g"`
LFLAGS=-shared `gdlib-config --ldflags` `gdlib-config --libs` -llua -lgd 


# Manual configuration for Windows and systems without sed and gdlib-config.
# Uncomment, change and good luck :)

#OUTFILE=libluagd.dll
#CFLAGS=-Wall -O3
#GDFEATURES=-DGD_XPM -DGD_JPEG -DGD_FONTCONFIG -DGD_FREETYPE -DGD_PNG -DGD_GIF
#LFLAGS=-shared -lgd2 -lm -llua -llualib


all: $(OUTFILE)

$(OUTFILE): luagd.c
	$(CC) -o $(OUTFILE) $(GDFEATURES) $(CFLAGS) $(LFLAGS) luagd.c
	lua test_features.lua

install: $(OUTFILE)
	strip $(OUTFILE)
	cp $(OUTFILE) /usr/lib/

install51: $(OUTFILE)
	strip $(OUTFILE)
	lua install51.lua $(OUTFILE) gd

clean:
	rm -f $(OUTFILE) *.o
