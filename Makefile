# luagd -- GD bindings to Lua.
# (c) 2004 Alexandre Erwin Ittner <aittner@netuno.com.br>
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
CFLAGS=-DUSE_XPM -DUSE_FONTCONFIG
LFLAGS=-shared -llua -lgd

all: libluagd.so

libluagd.so: luagd.c
	$(CC) -o libluagd.so $(CFLAGS) $(LFLAGS) luagd.c  -Wall

install: libluagd.so
	cp libluagd.so /usr/lib/

clean:
	rm -f libluagd.so *.o
