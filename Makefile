# luagd (c) 2004 Alexandre Erwin Ittner <aittner@netuno.com.br>
# GD bindings to Lua. Like Lua, this program is released under
# the MIT license (see http://www.lua.org/copyright.html).
# WITHOUT ANY WARRANTY.


CC=gcc

all: libluagd.so

libluagd.so: luagd.c
	$(CC) -o libluagd.so -shared luagd.c -llua -lgd2 -Wall

install: libluagd.so
	cp libluagd.so /usr/lib/

clean:
	rm -f libluagd.so *.o
