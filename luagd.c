/*
 * luagd (c) 2004 Alexandre Erwin Ittner <aittner@netuno.com.br>
 * GD bindings to Lua. Like Lua, this program is released under
 * the MIT license (see http://www.lua.org/copyright.html).
 * WITHOUT ANY WARRANTY.
 *
 * $Id$
 *
 */

#include <lua.h>
#include <lauxlib.h>
#include <gd.h>

#define LIBNAME                 "gd"
#define GD_IMAGE_TYPENAME       "gdImage_handle"
#define GD_IMAGE_PTR_TYPENAME   "gdImagePtr_handle"
#define GD_FONT_TYPENAME        "gdFont_handle"
#define GD_FONT_PTR_TYPENAME    "gdFontPtr_handle"


/* These macros was written by Luiz Henrique de Figueiredo
  <lhf@tecgraf.puc-rio.br> in his lpdf.c */

#define getstring           luaL_checkstring
#define getostring(L, i)    luaL_optstring(L, i, NULL)
#define getfloat            (float) luaL_checknumber
#define getofloat(L, i)     (float) luaL_optnumber(L, i, 0)
#define getint              (int) luaL_checknumber
#define getoint(L, i)       (int) luaL_optnumber(L, i, 0)
#define getlong             (long) luaL_checknumber





static gdImagePtr getImagePtr(lua_State *L, int i)
{
    if(luaL_checkudata(L, i, GD_IMAGE_PTR_TYPENAME) != NULL)
    {
        PDF *p = lua_unboxpointer(L, i);

        if(p == NULL)
            luaL_error(L, "attempt to use an invalid " GD_IMAGE_PTR_TYPENAME);
        return p;
    }
    luaL_typerror(L, i, GD_IMAGE_PTR_TYPENAME);
    return NULL;
}


/* gdImageCreate (int sx, int sy); */
static int LgdImageCreate(Lua_State *L)
{
    int sx, sy;
    


int luaopen_gd(lua_State *L)
{
    return 0;
}

