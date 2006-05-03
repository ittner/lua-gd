# luagd -- gd bindings for the Lua Programming Language.
# (c) 2004-06 Alexandre Erwin Ittner <aittner@netuno.com.br>
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# If you use this package in a product, an acknowledgment in the product
# documentation would be greatly appreciated (but it is not required).
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

#OUTFILE=gd.so
#CFLAGS=-Wall `gdlib-config --cflags` -I/usr/include/lua5.1 -O3
#GDFEATURES=`gdlib-config --features |sed -e "s/GD_/-DGD_/g"`
#LFLAGS=-shared `gdlib-config --ldflags` `gdlib-config --libs` -llua51 -lgd
#INSTALL_PATH=/usr/lib/lua/


# ---------------------------------------------------------------------------
# Manual configuration for Windows and systems without sed, pkgconfig, etc.
# Uncomment, change and good luck :)

#OUTFILE=gd.dll
#CFLAGS=-Wall -IC:/lua5.1/ -O3
#GDFEATURES=-DGD_XPM -DGD_JPEG -DGD_FONTCONFIG -DGD_FREETYPE -DGD_PNG -DGD_GIF
#LFLAGS=-shared -lgd2 -lm -llua51
#INSTALL_PATH="C:/Program Files/lua/"
# ---------------------------------------------------------------------------


all: $(OUTFILE)

$(OUTFILE): luagd.c
	$(CC) -o $(OUTFILE) $(GDFEATURES) $(CFLAGS) $(LFLAGS) luagd.c
	lua test_features.lua

install: $(OUTFILE)
	install -s $(OUTFILE) $(INSTALL_PATH)

clean:
	rm -f $(OUTFILE) *.o
