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

#define LIB_NAME                "gd"
#define MY_GD_VERSION           "2.17"
#define GD_IMAGE_TYPENAME       "gdImage_handle"
#define GD_IMAGE_PTR_TYPENAME   "gdImagePtr_handle"
#define GD_FONT_TYPENAME        "gdFont_handle"
#define GD_FONT_PTR_TYPENAME    "gdFontPtr_handle"

#define LIB_VERSION "lua-gd (c) 2004 Alexandre Erwin Ittner, based on " \
    "gd " MY_GD_VERSION


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
        gdImagePtr im = lua_unboxpointer(L, i);
        if(im == NULL)
            luaL_error(L, "attempt to use an invalid " GD_IMAGE_PTR_TYPENAME);
        return im;
    }
    luaL_typerror(L, i, GD_IMAGE_PTR_TYPENAME);
    return NULL;
}


/* gdImageCreate(int sx, int sy) */
static int LgdImageCreate(lua_State *L)
{
    int sx, sy;
    gdImagePtr im;

    sx = getint(L, 1);
    sy = getint(L, 2);
    im = gdImageCreate(sx, sy);
    if(im != NULL)
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
    else
        lua_pushnil(L); /* Error */
    return 1;
}

/* gdImageCreateTrueColor(int sx, int sy) */
static int LgdImageCreateTrueColor(lua_State *L)
{
    int sx, sy;
    gdImagePtr im;

    sx = getint(L, 1);
    sy = getint(L, 2);
    im = gdImageCreateTrueColor(sx, sy);
    if(im != NULL)
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
    else
        lua_pushnil(L); /* Error */
    return 1;
}

/* gdImageDestroy(gdImagePtr im) */
static int LgdImageDestroy(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    if(im)
        gdImageDestroy(im);
    return 0;
}

/* gdImageCreateFromJpeg(FILE *in) */
/* Changed to: gdImageCreateFromJpeg(char *filename) */
static int LgdImageCreateFromJpeg(lua_State *L)
{
    gdImagePtr im;
    FILE *fp;
    const char *fname = getstring(L, 1);

    if(fname == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    if((fp = fopen(fname, "rb")) == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromJpeg(fp);
    fclose(fp);
    if(im != NULL)
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
    else
        lua_pushnil(L); /* Error */
    return 1;
}



/* gdImageCreateFromJpegCtx(gdIOCtx *in) 

gdImageCreateFromPng(FILE *in) (FUNCTION)
gdImageCreateFromPngCtx(gdIOCtx *in) (FUNCTION)

gdImageCreateFromGd(FILE *in) (FUNCTION)
gdImageCreateFromGdCtx(gdIOCtx *in) (FUNCTION)

*/



/* gdImageJpeg(gdImagePtr im, FILE *out, int quality) */
/* Changed to: gdImageJpeg(gdImagePtr im, char *fname, int quality) */
static int LgdImageJpeg(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    const char *fname = getstring(L, 2);
    int quality = getint(L, 3);
    FILE *fp;

    if(fname == NULL)
    {
        lua_pushnil(L);
        return 1;
    }
    if((fp = fopen(fname, "wb")) == NULL)
    {
        lua_pushnil(L);
        return 1;
    }
    gdImageJpeg(im, fp, quality);
    fclose(fp);
    lua_pushnumber(L, 1);
    return 1;
}
    
static int LgdImageJpegPtr(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int quality = getint(L, 2);
    char *str;
    int size;

    str = gdImageJpegPtr(im, &size, quality);
    if(str != NULL)
    {
        lua_pushlstring(L, str, size);
        gdFree(str);
    }
    else
        lua_pushnil(L);  /* Error */
    return 1;
}




static const luaL_reg LgdFunctions[] =
{
	{ "ImageCreate",            LgdImageCreate },
	{ "ImageCreateTrueColor",   LgdImageCreateTrueColor },
	{ "ImageDestroy",           LgdImageDestroy },
	{ "ImageCreateFromJpeg",    LgdImageCreateFromJpeg },
	{ "ImageJpeg",              LgdImageJpeg },
    { "ImageJpegPtr",           LgdImageJpegPtr },
    { NULL, NULL }
};


int luaopen_gd(lua_State *L)
{
    luaL_openlib(L, LIB_NAME, LgdFunctions, 0);
    lua_pushliteral(L, "version");			/** version */
    lua_pushliteral(L, LIB_VERSION);
    lua_settable(L, -3);

    lua_pushliteral(L, "metatable");		/** metatable */
    luaL_newmetatable(L, GD_IMAGE_PTR_TYPENAME);
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -4);
    lua_settable(L, -3);

    return 0;
}

