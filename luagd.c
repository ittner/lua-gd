/*
 * luagd -- GD bindings to Lua.
 * (c) 2004 Alexandre Erwin Ittner <aittner@netuno.com.br>
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *
 * $Id$
 *
 */

#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>

#include <gd.h>

/* Standard gd fonts */
#include <gdfonts.h>
#include <gdfontl.h>
#include <gdfontmb.h>
#include <gdfontg.h>
#include <gdfontt.h>


#define LIB_NAME                "gd"
#define MY_GD_VERSION           "gd 2.0.33"
#define GD_IMAGE_TYPENAME       "gdImage_handle"
#define GD_IMAGE_PTR_TYPENAME   "gdImagePtr_handle"
#define GD_FONT_TYPENAME        "gdFont_handle"
#define GD_FONT_PTR_TYPENAME    "gdFontPtr_handle"

#define LIB_VERSION "lua-gd " MY_GD_VERSION " (c) 2004 Alexandre Erwin Ittner"


/* These macros was written by Luiz Henrique de Figueiredo
  <lhf@tecgraf.puc-rio.br> in his lpdf.c */

#define getstring           luaL_checkstring
#define getostring(L, i)    luaL_optstring(L, i, NULL)
#define getfloat            (float) luaL_checknumber
#define getofloat(L, i)     (float) luaL_optnumber(L, i, 0)
#define getint              (int) luaL_checknumber
#define getoint(L, i)       (int) luaL_optnumber(L, i, 0)
#define getlong             (long) luaL_checknumber

/* Table assumed on top */
#define tblseticons(L, c, v)    \
    lua_pushliteral(L, c);      \
    lua_pushnumber(L, v);       \
    lua_settable(L, -3);

/* Standard gd fonts */
#define MY_GD_FONT_SMALL            0
#define MY_GD_FONT_LARGE            1
#define MY_GD_FONT_MEDIUM_BOLD      2
#define MY_GD_FONT_GIANT            3
#define MY_GD_FONT_TINY             4


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


static void pushImagePtr(lua_State *L, gdImagePtr im)
{
    lua_boxpointer(L, im);
    luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
    lua_setmetatable(L, -2);    /* Done */
}


static gdFontPtr getStdFont(lua_State *L, int i)
{
    int size;

    if(lua_isnumber(L, i) == 0)
    {
        luaL_typerror(L, i, "Standard GD Font");
        return gdFontGetSmall();
    }

    size = getint(L, i);
    switch(size)
    {
        case MY_GD_FONT_SMALL:
            return gdFontGetSmall();

        case MY_GD_FONT_LARGE:
            return gdFontGetLarge();

        case MY_GD_FONT_MEDIUM_BOLD:
            return gdFontGetMediumBold();

        case MY_GD_FONT_GIANT:
            return gdFontGetGiant();

        case MY_GD_FONT_TINY:
            return gdFontGetTiny();

        default:
            luaL_typerror(L, i, "Standard GD Font");
            return gdFontGetSmall();
    }

    return gdFontGetSmall(); /* Not reached */
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
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}

/* gdImageCreatePalette(int sx, int sy) */
/* Useless? */
static int LgdImageCreatePalette(lua_State *L)
{
    int sx, sy;
    gdImagePtr im;

    sx = getint(L, 1);
    sy = getint(L, 2);
    im = gdImageCreatePalette(sx, sy);
    if(im != NULL)
        pushImagePtr(L, im);
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
        pushImagePtr(L, im);
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
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromJpegPtr(int size, void *data) */
static int LgdImageCreateFromJpegPtr(lua_State *L)
{
    gdImagePtr im;
    int size = lua_strlen(L, 1);
    void *str = (void*) getstring(L, 1);

    if(str == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromJpegPtr(size, str);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}



/* gdImageCreateFromGif(FILE *in) */
/* Changed to: gdImageCreateFromGif(char *filename) */
static int LgdImageCreateFromGif(lua_State *L)
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
    im = gdImageCreateFromGif(fp);
    fclose(fp);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromGifPtr(int size, void *data) */
static int LgdImageCreateFromGifPtr(lua_State *L)
{
    gdImagePtr im;
    int size = lua_strlen(L, 1);
    void *str = (void*) getstring(L, 1);

    if(str == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromGifPtr(size, str);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}



/* gdImageCreateFromPng(FILE *in) */
/* Changed to: gdImageCreateFromPng(char *filename) */
static int LgdImageCreateFromPng(lua_State *L)
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
    im = gdImageCreateFromPng(fp);
    fclose(fp);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromPngPtr(int size, void *data) */
static int LgdImageCreateFromPngPtr(lua_State *L)
{
    gdImagePtr im;
    int size = lua_strlen(L, 1);
    void *str = (void*) getstring(L, 1);

    if(str == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromPngPtr(size, str);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromGd(FILE *in) */
/* Changed to: gdImageCreateFromGd(char *filename) */
static int LgdImageCreateFromGd(lua_State *L)
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
    im = gdImageCreateFromGd(fp);
    fclose(fp);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromGdPtr(int size, void *data) */
static int LgdImageCreateFromGdPtr(lua_State *L)
{
    gdImagePtr im;
    int size = lua_strlen(L, 1);
    void *str = (void*) getstring(L, 1);

    if(str == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromGdPtr(size, str);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromGd2(FILE *in) */
/* Changed to: gdImageCreateFromGd2(char *filename) */
static int LgdImageCreateFromGd2(lua_State *L)
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
    im = gdImageCreateFromGd2(fp);
    fclose(fp);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromGd2Ptr(int size, void *data) */
static int LgdImageCreateFromGd2Ptr(lua_State *L)
{
    gdImagePtr im;
    int size = lua_strlen(L, 1);
    void *str = (void*) getstring(L, 1);

    if(str == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromGd2Ptr(size, str);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromGd2Part(FILE *in, int x, int y, int w, int h) */
/* Changed to: gdImageCreateFromGd2Part(char *filename, int x, int y,
                    int w, int h)) */
static int LgdImageCreateFromGd2Part(lua_State *L)
{
    gdImagePtr im;
    FILE *fp;
    const char *fname = getstring(L, 1);
    const int x = getint(L, 2);
    const int y = getint(L, 3);
    const int w = getint(L, 4);
    const int h = getint(L, 5);

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
    im = gdImageCreateFromGd2Part(fp, x, y, w, h);
    fclose(fp);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromGd2PartPtr(int size, void *data,
                int srcX, int srcY, int w, int h)  */
static int LgdImageCreateFromGd2PartPtr(lua_State *L)
{
    gdImagePtr im;
    int size = lua_strlen(L, 1);
    void *str = (void*) getstring(L, 1);
    const int x = getint(L, 2);
    const int y = getint(L, 3);
    const int w = getint(L, 4);
    const int h = getint(L, 5);

    if(str == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromGd2PartPtr(size, str, x, y, w, h);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromXbm(FILE *in) */
/* Changed to: gdImageCreateFromXbm(char *filename) */
static int LgdImageCreateFromXbm(lua_State *L)
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
    im = gdImageCreateFromXbm(fp);
    fclose(fp);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}


/* gdImageCreateFromXpm(char *filename) */
static int LgdImageCreateFromXpm(lua_State *L)
{
    gdImagePtr im;
    char *fname = (char*) getstring(L, 1);

    if(fname == NULL)
    {
        lua_pushnil(L);
        return 1;  /* Error */
    }
    im = gdImageCreateFromXpm(fname);
    if(im != NULL)
        pushImagePtr(L, im);
    else
        lua_pushnil(L); /* Error */
    return 1;
}





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


/* void *gdImageJpegPtr(gdImagePtr im, int quality) */
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


/* gdImagePng(gdImagePtr im, FILE *out) */
/* Changed to: gdImagePng(gdImagePtr im, char *fname) */
static int LgdImagePng(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    const char *fname = getstring(L, 2);
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
    gdImagePng(im, fp);
    fclose(fp);
    lua_pushnumber(L, 1);
    return 1;
}


/* void *gdImagePngPtr(gdImagePtr im) */
static int LgdImagePngPtr(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    char *str;
    int size;

    str = gdImagePngPtr(im, &size);
    if(str != NULL)
    {
        lua_pushlstring(L, str, size);
        gdFree(str);
    }
    else
        lua_pushnil(L);  /* Error */
    return 1;
}


/* gdImagePngEx(gdImagePtr im, FILE *out, int compression_level) */
/* Changed to: gdImagePngEx(gdImagePtr im, char *fname, int compr_level) */
static int LgdImagePngEx(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    const char *fname = getstring(L, 2);
    int level = getint(L, 3);
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
    gdImagePngEx(im, fp, level);
    fclose(fp);
    lua_pushnumber(L, 1);
    return 1;
}


/* void *gdImagePngPtrEx(gdImagePtr im, int compression_level) */
static int LgdImagePngPtrEx(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int level = getint(L, 2);
    char *str;
    int size;

    str = gdImagePngPtrEx(im, &size, level);
    if(str != NULL)
    {
        lua_pushlstring(L, str, size);
        gdFree(str);
    }
    else
        lua_pushnil(L);  /* Error */
    return 1;
}



/* gdImageGif(gdImagePtr im, FILE *out) */
/* Changed to: gdImageGif(gdImagePtr im, char *fname) */
static int LgdImageGif(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    const char *fname = getstring(L, 2);
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
    gdImageGif(im, fp);
    fclose(fp);
    lua_pushnumber(L, 1);
    return 1;
}


/* void *gdImageGifPtr(gdImagePtr im) */
static int LgdImageGifPtr(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    char *str;
    int size;

    str = gdImageGifPtr(im, &size);
    if(str != NULL)
    {
        lua_pushlstring(L, str, size);
        gdFree(str);
    }
    else
        lua_pushnil(L);  /* Error */
    return 1;
}



/* gdImageGd(gdImagePtr im, FILE *out) */
/* Changed to: gdImageGd(gdImagePtr im, char *fname) */
static int LgdImageGd(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    const char *fname = getstring(L, 2);
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
    gdImageGd(im, fp);
    fclose(fp);
    lua_pushnumber(L, 1);
    return 1;
}


/* void *gdImageGdPtr(gdImagePtr im) */
static int LgdImageGdPtr(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    char *str;
    int size;

    str = gdImageGdPtr(im, &size);
    if(str != NULL)
    {
        lua_pushlstring(L, str, size);
        gdFree(str);
    }
    else
        lua_pushnil(L);  /* Error */
    return 1;
}



/* gdImageGd2(gdImagePtr im, FILE *out, int chunkSize, int fmt) */
/* Changed to: gdImageGd2(gdImagePtr im, char *fname, int cs, int fmt) */
static int LgdImageGd2(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    const char *fname = getstring(L, 2);
    int cs = getint(L, 3);
    int fmt = getint(L, 4);
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
    gdImageGd2(im, fp, cs, fmt);
    fclose(fp);
    lua_pushnumber(L, 1);
    return 1;
}


/* void* gdImageGd2Ptr(gdImagePtr im, int chunkSize, int fmt, int *size) */
static int LgdImageGd2Ptr(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int cs = getint(L, 2);
    int fmt = getint(L, 3);
    char *str;
    int size;

    str = gdImageGd2Ptr(im, cs, fmt, &size);
    if(str != NULL)
    {
        lua_pushlstring(L, str, size);
        gdFree(str);
    }
    else
        lua_pushnil(L);  /* Error */
    return 1;
}


/* void gdImageWBMP(gdImagePtr im, int fg, FILE *out) */
/* Changed to: gdImageWBMP(gdImagePtr im, int fg, char *fname) */
static int LgdImageWBMP(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int fg = getint(L, 2);
    const char *fname = getstring(L, 3);
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
    gdImageWBMP(im, fg, fp);
    fclose(fp);
    lua_pushnumber(L, 1);
    return 1;
}


/* void* gdImageWBMPPtr(gdImagePtr im, int *size) */
static int LgdImageWBMPPtr(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int fg = getint(L, 2);
    char *str;
    int size;

    str = gdImageWBMPPtr(im, &size, fg);
    if(str != NULL)
    {
        lua_pushlstring(L, str, size);
        gdFree(str);
    }
    else
        lua_pushnil(L);  /* Error */
    return 1;
}


/* int gdImageColorAllocate(gdImagePtr im, int r, int g, int b) */
static int LgdImageColorAllocate(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int c;

    if(im)
    {
        c = gdImageColorAllocate(im, r, g, b);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorAllocateAlpha(gdImagePtr im, int r, int g, int b, int a) */
static int LgdImageColorAllocateAlpha(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int a = getint(L, 5);
    int c;

    if(im)
    {
        c = gdImageColorAllocateAlpha(im, r, g, b, a);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorClosest(gdImagePtr im, int r, int g, int b) */
static int LgdImageColorClosest(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int c;

    if(im)
    {
        c = gdImageColorClosest(im, r, g, b);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorClosestAlpha(gdImagePtr im, int r, int g, int b, int a) */
static int LgdImageColorClosestAlpha(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int a = getint(L, 5);
    int c;

    if(im)
    {
        c = gdImageColorClosestAlpha(im, r, g, b, a);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorClosestHWB(gdImagePtr im, int r, int g, int b) */
static int LgdImageColorClosestHWB(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int c;

    if(im)
    {
        c = gdImageColorClosestHWB(im, r, g, b);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorExact(gdImagePtr im, int r, int g, int b) */
static int LgdImageColorExact(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int c;

    if(im)
    {
        c = gdImageColorExact(im, r, g, b);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorExactAlpha(gdImagePtr im, int r, int g, int b, int a) */
static int LgdImageColorExactAlpha(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int a = getint(L, 5);
    int c;

    if(im)
    {
        c = gdImageColorExactAlpha(im, r, g, b, a);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorResolve(gdImagePtr im, int r, int g, int b) */
static int LgdImageColorResolve(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int c;

    if(im)
    {
        c = gdImageColorResolve(im, r, g, b);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorResolveAlpha(gdImagePtr im, int r, int g, int b, int a) */
static int LgdImageColorResolveAlpha(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);
    int g = getint(L, 3);
    int b = getint(L, 4);
    int a = getint(L, 5);
    int c;

    if(im)
    {
        c = gdImageColorResolveAlpha(im, r, g, b, a);
        if(c > 0)
            lua_pushnumber(L, c);  /* ok */
        else
            lua_pushnil(L); /* Can not allocate color */
    }
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageColorsTotal(gdImagePtr im) */
static int LgdImageColorsTotal(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);

    if(im)
        lua_pushnumber(L, gdImageColorsTotal(im));  /* ok */
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}


/* int gdImageRed(gdImagePtr im, int c) */
static int LgdImageRed(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);

    if(im)
        lua_pushnumber(L, gdImageRed(im, c));  /* ok */
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}

/* int gdImageBlue(gdImagePtr im, int c) */
static int LgdImageBlue(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);

    if(im)
        lua_pushnumber(L, gdImageBlue(im, c));  /* ok */
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}

/* int gdImageBlue(gdImagePtr im, int c) */
static int LgdImageGreen(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);

    if(im)
        lua_pushnumber(L, gdImageGreen(im, c));  /* ok */
    else
        lua_pushnil(L);  /* Error reading the image */
    return 1;
}

/* int gdImageAlpha(gdImagePtr im, int color) */
static int LgdImageAlpha(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);
    lua_pushnumber(L, gdImageAlpha(im, c));
    return 1;
}

/* int gdImageGetInterlaced(gdImagePtr im) */
static int LgdImageGetInterlaced(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int ret = gdImageGetInterlaced(im);

    if(ret != 0)
        lua_pushnumber(L, ret);
    else
        lua_pushnil(L);
    return 1;
}

/* int gdImageGetTransparent(gdImagePtr im) */
static int LgdImageGetTransparent(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int ret = gdImageGetTransparent(im);

    if(ret != -1)
        lua_pushnumber(L, ret);
    else
        lua_pushnil(L);
    return 1;
}


/* void gdImageColorTransparent(gdImagePtr im, int c) */
static int LgdImageColorTransparent(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);
    gdImageColorTransparent(im, c);
    return 0;
}


/* void gdImageColorDeallocate(gdImagePtr im, int c) */
static int LgdImageColorDeallocate(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);
    gdImageColorDeallocate(im, c);
    return 0;
}


/* int gdImageSX(gdImagePtr im) */
static int LgdImageSX(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    lua_pushnumber(L, gdImageSX(im));
    return 1;
}


/* int gdImageSY(gdImagePtr im) */
static int LgdImageSY(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    lua_pushnumber(L, gdImageSY(im));
    return 1;
}


/* Fear the power of the Moon!!  ---   x, y = im:ImageSXY() */
static int LgdImageSXY(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    lua_pushnumber(L, gdImageSX(im));
    lua_pushnumber(L, gdImageSY(im));
    return 2;
}


/* int gdImageBoundsSafe(gdImagePtr im, int x, int y) */
static int LgdImageBoundsSafe(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x = getint(L, 2);
    int y = getint(L, 3);

    if(gdImageBoundsSafe(im, x, y) != 0)
        lua_pushnumber(L, 1);
    else
        lua_pushnil(L);
    return 1;
}


/* int gdImageGetPixel(gdImagePtr im, int x, int y) */
static int LgdImageGetPixel(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x = getint(L, 2);
    int y = getint(L, 3);

    if(im)
        lua_pushnumber(L, gdImageGetPixel(im, x, y));
    return 1;
}


/* void gdImageSetPixel(gdImagePtr im, int x, int y, int color) */
static int LgdImageSetPixel(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x = getint(L, 2);
    int y = getint(L, 3);
    int c = getint(L, 4);

    if(im)
        gdImageSetPixel(im, x, y, c);
    return 0;
}


/* void gdImageLine(gdImagePtr im, int x1, int y1, int x2, int y2, int c) */
static int LgdImageLine(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x1 = getint(L, 2);
    int y1 = getint(L, 3);
    int x2 = getint(L, 4);
    int y2 = getint(L, 5);
    int c = getint(L, 6);

    if(im)
        gdImageLine(im, x1, y1, x2, y2, c);
    return 0;
}

/* void gdImageRectangle(gdImagePtr im, int x1, int y1, int x2, int y2,
        int c) */
static int LgdImageRectangle(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x1 = getint(L, 2);
    int y1 = getint(L, 3);
    int x2 = getint(L, 4);
    int y2 = getint(L, 5);
    int c = getint(L, 6);

    if(im)
        gdImageRectangle(im, x1, y1, x2, y2, c);
    return 0;
}


/* void gdImageFilledRectangle(gdImagePtr im, int x1, int y1, int x2, int y2,
        int c) */
static int LgdImageFilledRectangle(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x1 = getint(L, 2);
    int y1 = getint(L, 3);
    int x2 = getint(L, 4);
    int y2 = getint(L, 5);
    int c = getint(L, 6);

    if(im)
        gdImageFilledRectangle(im, x1, y1, x2, y2, c);
    return 0;
}


/* void gdImagePolygon(gdImagePtr im, gdPointPtr points, int pointsTotal,
        int color)
  Changed to: ImagePolygon(im, { { x1, y1 }, { x2, y2 } ... }, color)  */
static int LgdImagePolygon(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdPoint *plist;
    int size;
    int i;
    int c;

    c = getint(L, 3);
    lua_remove(L, 3); /* Get and drop color */
    lua_remove(L, 1); /* Drop image from the stack */

    luaL_checktype(L, -1, LUA_TTABLE);
    size = luaL_getn(L, -1);
    plist = (gdPoint*) malloc(size * sizeof(gdPoint));

    for(i = 0; i < size; i++)
    {
        /* Stack: T */
        lua_rawgeti(L, 1, i + 1);

        /* Stack:  T, T'  */
        if(lua_type(L, 2) != LUA_TTABLE)
        {
            free(plist);
            luaL_typerror(L, 2, "Point");
        }

        lua_rawgeti(L, 2, 1);
        /* Stack:  T, T', X  */
        plist[i].x = getint(L, -1);
        lua_remove(L, -1);

        /* Stack:  T, T', Y  */
        lua_rawgeti(L, 2, 2);
        plist[i].y = getint(L, -1);
        lua_remove(L, -1);

        /* Stack:  T, T' */
        lua_remove(L, -1);

        /* Stack: T */
    }

    gdImagePolygon(im, plist, size, c);
    free(plist);
    return 0;
}



/* void gdImageFilledPolygon(gdImagePtr im, gdPointPtr points,
        int pointsTotal, int color)
  Changed to: ImageFilledPolygon(im, { { x1, y1 }, { x2, y2 } ... }, color) */
static int LgdImageFilledPolygon(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdPoint *plist;
    int size;
    int i;
    int c;

    c = getint(L, 3);
    lua_remove(L, 3); /* Get and drop color */
    lua_remove(L, 1); /* Drop image from the stack */

    luaL_checktype(L, -1, LUA_TTABLE);
    size = luaL_getn(L, -1);
    plist = (gdPoint*) malloc(size * sizeof(gdPoint));

    for(i = 0; i < size; i++)
    {
        /* Stack: T */
        lua_rawgeti(L, 1, i + 1);

        /* Stack:  T, T'  */
        if(lua_type(L, 2) != LUA_TTABLE)
        {
            free(plist);
            luaL_typerror(L, 2, "Point");
        }

        lua_rawgeti(L, 2, 1);
        /* Stack:  T, T', X  */
        plist[i].x = getint(L, -1);
        lua_remove(L, -1);

        /* Stack:  T, T', Y  */
        lua_rawgeti(L, 2, 2);
        plist[i].y = getint(L, -1);
        lua_remove(L, -1);

        /* Stack:  T, T' */
        lua_remove(L, -1);

        /* Stack: T */
    }

    gdImageFilledPolygon(im, plist, size, c);
    free(plist);
    return 0;
}


/* void gdImageArc(gdImagePtr im, int cx, int cy, int w, int h, int s, int e,
    int color) */
static int LgdImageArc(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int cx = getint(L, 2);
    int cy = getint(L, 3);
    int w = getint(L, 4);
    int h = getint(L, 5);
    int s = getint(L, 6);
    int e = getint(L, 7);
    int c = getint(L, 8);

    gdImageArc(im, cx, cy, w, h, s, e, c);
    return 0;
}


/* void gdImageFilledArc(gdImagePtr im, int cx, int cy, int w, int h,
    int s, int e, int color, int style) */
static int LgdImageFilledArc(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int cx = getint(L, 2);
    int cy = getint(L, 3);
    int w = getint(L, 4);
    int h = getint(L, 5);
    int s = getint(L, 6);
    int e = getint(L, 7);
    int c = getint(L, 8);
    int sty = getint(L, 9);

    gdImageFilledArc(im, cx, cy, w, h, s, e, c, sty);
    return 0;
}

/* void gdImageEllipse(gdImagePtr im, int cx, int cy, int w, int h,
        int color) */
static int LgdImageEllipse(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int cx = getint(L, 2);
    int cy = getint(L, 3);
    int w = getint(L, 4);
    int h = getint(L, 5);
    int c = getint(L, 6);

    gdImageFilledEllipse(im, cx, cy, w, h, c);
    return 0;
}


/* void gdImageFilledEllipse(gdImagePtr im, int cx, int cy, int w, int h,
        int color) */
static int LgdImageFilledEllipse(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int cx = getint(L, 2);
    int cy = getint(L, 3);
    int w = getint(L, 4);
    int h = getint(L, 5);
    int c = getint(L, 6);

    gdImageFilledEllipse(im, cx, cy, w, h, c);
    return 0;
}


/* void gdImageFill(gdImagePtr im, int x, int y, int color) */
static int LgdImageFill(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x = getint(L, 2);
    int y = getint(L, 3);
    int c = getint(L, 4);

    gdImageFill(im, x, y, c);
    return 0;
}


/* void gdImageFillToBorder(gdImagePtr im, int x, int y, int border,
        int color) */
static int LgdImageFillToBorder(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int x = getint(L, 2);
    int y = getint(L, 3);
    int b = getint(L, 4);
    int c = getint(L, 5);

    gdImageFillToBorder(im, x, y, b, c);
    return 0;
}


/* void gdImageSetAntiAliased(gdImagePtr im, int c) */
static int LgdImageSetAntiAliased(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);

    gdImageSetAntiAliased(im, c);
    return 0;
}


/* void gdImageSetAntiAliasedDontBlend(gdImagePtr im, int c) */
static int LgdImageSetAntiAliasedDontBlend(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int c = getint(L, 2);

    gdImageSetAntiAliasedDontBlend(im, c, 1);
    return 0;
}


/* void gdImageSetBrush(gdImagePtr im, gdImagePtr brush) */
static int LgdImageSetBrush(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdImagePtr b = getImagePtr(L, 2);

    gdImageSetBrush(im, b);
    return 0;
}


/* void gdImageSetTile(gdImagePtr im, gdImagePtr tile) */
static int LgdImageSetTile(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdImagePtr t = getImagePtr(L, 1);

    gdImageSetTile(im, t);
    return 0;
}


/* void gdImageSetStyle(gdImagePtr im, int *style, int styleLength)  */
/* Changed To: im:ImageSetStyle( { c1, c2, c3,  ...  } ) */

static int LgdImageSetStyle(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int *slist;
    int size;
    int i;

    /* Stack: Im, T */
    luaL_checktype(L, -1, LUA_TTABLE);
    size = luaL_getn(L, -1);
    slist = (int*) malloc(size * sizeof(int));

    for(i = 0; i < size; i++)
    {
        /* Stack: Im, T */
        lua_rawgeti(L, 2, i + 1);

        /* Stack:  Im, T, num */
        if(lua_type(L, -1) != LUA_TNUMBER)
        {
            free(slist);
            luaL_typerror(L, -1, "Number");
        }

        slist[i] = getint(L, -1);
        lua_remove(L, -1);

        /* Stack: Im, T */
    }

    gdImageSetStyle(im, slist, size);
    free(slist);

    return 0;
}


/* void gdImageSetThickness(gdImagePtr im, int thickness) */
static int LgdImageSetThickness(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int t = getint(L, 2);

    gdImageSetThickness(im, t);
    return 0;
}


/* void gdImageAlphaBlending(gdImagePtr im, int blending) */
/* Changed to: im:ImageAlphaBlending(true_or_false) */
static int LgdImageAlphaBlending(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int b = lua_toboolean(L, 2);

    gdImageAlphaBlending(im, b);
    return 0;
}


/* void gdImageSaveAlpha(gdImagePtr im, int saveFlag) */
/* Changed to: im:ImageSaveAlpha(true_or_false) */
static int LgdImageSaveAlpha(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int b = lua_toboolean(L, 2);

    gdImageSaveAlpha(im, b);
    return 0;
}


/* gdImageInterlace(gdImagePtr im, int interlace) */
/* Changed to: im:ImageInterlace(true_or_false) */
static int LgdImageInterlace(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int i = lua_toboolean(L, 2);

    gdImageInterlace(im, i);
    return 0;
}


/* void gdImageString(gdImagePtr im, gdFontPtr font, int x, int y,
        unsigned char *s, int color) */
static int LgdImageString(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdFontPtr fnt = getStdFont(L, 2);
    int x = getint(L, 3);
    int y = getint(L, 4);
    char *str = (char*) getstring(L, 5);
    int c = getint(L, 6);

    gdImageString(im, fnt, x, y, str, c);
    return 0;
}


/* void gdImageStringUp(gdImagePtr im, gdFontPtr font, int x, int y,
        unsigned char *s, int color)  */
static int LgdImageStringUp(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdFontPtr fnt = getStdFont(L, 2);
    int x = getint(L, 3);
    int y = getint(L, 4);
    char *str = (char*) getstring(L, 5);
    int c = getint(L, 6);

    gdImageStringUp(im, fnt, x, y, str, c);
    return 0;
}


/* void gdImageChar(gdImagePtr im, gdFontPtr font, int x, int y,
            int c, int color)  */
/* Useless? */
static int LgdImageChar(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdFontPtr fnt = getStdFont(L, 2);
    int x = getint(L, 3);
    int y = getint(L, 4);
    char *str = (char*) getstring(L, 5);
    int c = getint(L, 6);
    int chr;

    if(str)
        chr = (int) str[0];
    else
    {
        luaL_typerror(L, 5, "string");
        return 0;
    }

    gdImageChar(im, fnt, x, y, chr, c);
    return 0;
}


/* void gdImageCharUp(gdImagePtr im, gdFontPtr font, int x, int y,
            int c, int color)  */
/* Useless? */
static int LgdImageCharUp(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    gdFontPtr fnt = getStdFont(L, 2);
    int x = getint(L, 3);
    int y = getint(L, 4);
    char *str = (char*) getstring(L, 5);
    int c = getint(L, 6);
    int chr;

    if(str)
        chr = (int) str[0];
    else
    {
        luaL_typerror(L, 5, "string");
        return 0;
    }

    gdImageCharUp(im, fnt, x, y, chr, c);
    return 0;
}


/* void gdImageCopy(gdImagePtr dst, gdImagePtr src, int dstX, int dstY,
            int srcX, int srcY, int w, int h) */
static int LgdImageCopy(lua_State *L)
{
    gdImagePtr dst = getImagePtr(L, 1);
    gdImagePtr src = getImagePtr(L, 2);
    int dstX = getint(L, 3);
    int dstY = getint(L, 4);
    int srcX = getint(L, 5);
    int srcY = getint(L, 6);
    int w = getint(L, 7);
    int h = getint(L, 8);

    gdImageCopy(dst, src, dstX, dstY, srcX, srcY, w, h);
    return 0;
}



/* void gdImageCopyResized(gdImagePtr dst, gdImagePtr src, int dstX,
            int dstY, int srcX, int srcY, int destW, int destH,
            int srcW, int srcH)  */
static int LgdImageCopyResized(lua_State *L)
{
    gdImagePtr dst = getImagePtr(L, 1);
    gdImagePtr src = getImagePtr(L, 2);
    int dstX = getint(L, 3);
    int dstY = getint(L, 4);
    int srcX = getint(L, 5);
    int srcY = getint(L, 6);
    int dstW = getint(L, 7);
    int dstH = getint(L, 8);
    int srcW = getint(L, 9);
    int srcH = getint(L, 10);

    gdImageCopyResized(dst, src, dstX, dstY, srcX, srcY, dstW, dstH,
        srcW, srcH);
    return 0;
}


/* void gdImageCopyResampled(gdImagePtr dst, gdImagePtr src, int dstX,
        int dstY, int srcX, int srcY, int destW, int destH, int srcW,
        int srcH)  */
static int LgdImageCopyResampled(lua_State *L)
{
    gdImagePtr dst = getImagePtr(L, 1);
    gdImagePtr src = getImagePtr(L, 2);
    int dstX = getint(L, 3);
    int dstY = getint(L, 4);
    int srcX = getint(L, 5);
    int srcY = getint(L, 6);
    int dstW = getint(L, 7);
    int dstH = getint(L, 8);
    int srcW = getint(L, 9);
    int srcH = getint(L, 10);

    gdImageCopyResampled(dst, src, dstX, dstY, srcX, srcY, dstW, dstH,
        srcW, srcH);
    return 0;
}


/* void gdImageCopyRotated(gdImagePtr dst, gdImagePtr src, double dstX,
        double dstY, int srcX, int srcY, int srcW, int srcH, int angle) */
static int LgdImageCopyRotated(lua_State *L)
{
    gdImagePtr dst = getImagePtr(L, 1);
    gdImagePtr src = getImagePtr(L, 2);
    double dstX = (double) lua_tonumber(L, 3);
    double dstY = (double) lua_tonumber(L, 4);
    int srcX = getint(L, 5);
    int srcY = getint(L, 6);
    int srcW = getint(L, 7);
    int srcH = getint(L, 8);
    int ang = getint(L, 9);

    gdImageCopyRotated(dst, src, dstX, dstY, srcX, srcY, srcW, srcH, ang);
    return 0;
}


/* void gdImageCopyMerge(gdImagePtr dst, gdImagePtr src, int dstX,
        int dstY, int srcX, int srcY, int w, int h, int pct)  */
static int LgdImageCopyMerge(lua_State *L)
{
    gdImagePtr dst = getImagePtr(L, 1);
    gdImagePtr src = getImagePtr(L, 2);
    int dstX = getint(L, 3);
    int dstY = getint(L, 4);
    int srcX = getint(L, 5);
    int srcY = getint(L, 6);
    int w = getint(L, 7);
    int h = getint(L, 8);
    int pct = getint(L, 9);

    gdImageCopyMerge(dst, src, dstX, dstY, srcX, srcY, w, h, pct);
    return 0;
}


/* void gdImageCopyMergeGray(gdImagePtr dst, gdImagePtr src, int dstX,
        int dstY, int srcX, int srcY, int w, int h, int pct) */
static int LgdImageCopyMergeGray(lua_State *L)
{
    gdImagePtr dst = getImagePtr(L, 1);
    gdImagePtr src = getImagePtr(L, 2);
    int dstX = getint(L, 3);
    int dstY = getint(L, 4);
    int srcX = getint(L, 5);
    int srcY = getint(L, 6);
    int w = getint(L, 7);
    int h = getint(L, 8);
    int pct = getint(L, 9);

    gdImageCopyMergeGray(dst, src, dstX, dstY, srcX, srcY, w, h, pct);
    return 0;
}


/* void gdImagePaletteCopy(gdImagePtr dst, gdImagePtr src) */
static int LgdImagePaletteCopy(lua_State *L)
{
    gdImagePtr dst = getImagePtr(L, 1);
    gdImagePtr src = getImagePtr(L, 2);

    gdImagePaletteCopy(dst, src);
    return 0;
}


/* void gdImageSquareToCircle(gdImagePtr im, int radius) */
static int LgdImageSquareToCircle(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int r = getint(L, 2);

    gdImageSquareToCircle(im, r);
    return 0;
}


/* void gdImageSharpen(gdImagePtr im, int pct) */
static int LgdImageSharpen(lua_State *L)
{
    gdImagePtr im = getImagePtr(L, 1);
    int pct = getint(L, 2);

    gdImageSharpen(im, pct);
    return 0;
}



static const luaL_reg LgdFunctions[] =
{
/*  Leave Lua do it!
    { "ImageDestroy",               LgdImageDestroy }, */

    { "ImageCreate",                LgdImageCreate },
    { "ImageCreatePalette",         LgdImageCreatePalette },
    { "ImageCreateTrueColor",       LgdImageCreateTrueColor },

    { "ImageCreateFromJpeg",        LgdImageCreateFromJpeg },
    { "ImageCreateFromJpegPtr",     LgdImageCreateFromJpegPtr },
    { "ImageCreateFromGif",         LgdImageCreateFromGif },
    { "ImageCreateFromGifPtr",      LgdImageCreateFromGifPtr },
    { "ImageCreateFromPng",         LgdImageCreateFromPng },
    { "ImageCreateFromPngPtr",      LgdImageCreateFromPngPtr },
    { "ImageCreateFromGd",          LgdImageCreateFromGd },
    { "ImageCreateFromGdPtr",       LgdImageCreateFromGdPtr },
    { "ImageCreateFromGd2",         LgdImageCreateFromGd2 },
    { "ImageCreateFromGd2Ptr",      LgdImageCreateFromGd2Ptr },
    { "ImageCreateFromGd2Part",     LgdImageCreateFromGd2Part },
    { "ImageCreateFromGd2PartPtr",  LgdImageCreateFromGd2PartPtr },
    { "ImageCreateFromXbm",         LgdImageCreateFromXbm },
    { "ImageCreateFromXpm",         LgdImageCreateFromXpm },

    { "ImageJpeg",                  LgdImageJpeg },
    { "ImageJpegPtr",               LgdImageJpegPtr },
    { "ImagePng",                   LgdImagePng },
    { "ImagePngPtr",                LgdImagePngPtr },
    { "ImagePngEx",                 LgdImagePngEx },
    { "ImagePngPtrEx",              LgdImagePngPtrEx },
    { "ImageGif",                   LgdImageGif },
    { "ImageGifPtr",                LgdImageGifPtr },
    { "ImageGd",                    LgdImageGd },
    { "ImageGdPtr",                 LgdImageGdPtr },
    { "ImageGd2",                   LgdImageGd2 },
    { "ImageGd2Ptr",                LgdImageGd2Ptr },
    { "ImageWBMP",                  LgdImageWBMP },
    { "ImageWBMPPtr",               LgdImageWBMPPtr },

    { "ImageColorAllocate",         LgdImageColorAllocate },
    { "ImageColorAllocateAlpha",    LgdImageColorAllocateAlpha },
    { "ImageColorClosest",          LgdImageColorClosest },
    { "ImageColorClosestAlpha",     LgdImageColorClosestAlpha },
    { "ImageColorClosestHWB",       LgdImageColorClosestHWB },
    { "ImageColorExact",            LgdImageColorExact },
    { "ImageColorExactAlpha",       LgdImageColorExactAlpha },
    { "ImageColorResolve",          LgdImageColorResolve },
    { "ImageColorResolveAlpha",     LgdImageColorResolveAlpha },
    { "ImageColorsTotal",           LgdImageColorsTotal },
    { "ImageRed",                   LgdImageRed },
    { "ImageBlue",                  LgdImageBlue },
    { "ImageGreen",                 LgdImageGreen },
    { "ImageAlpha",                 LgdImageAlpha },
    { "ImageGetTransparent",        LgdImageGetTransparent },
    { "ImageColorTransparent",      LgdImageColorTransparent },
    { "ImageColorDeallocate",       LgdImageColorDeallocate },

    { "ImageBoundsSafe",            LgdImageBoundsSafe },
    { "ImageGetPixel",              LgdImageGetPixel },
    { "ImageSX",                    LgdImageSX },
    { "ImageSY",                    LgdImageSY },
    { "ImageSXY",                   LgdImageSXY },

    { "ImageSetPixel",              LgdImageSetPixel },
    { "ImageLine",                  LgdImageLine },
    { "ImageRectangle",             LgdImageRectangle },
    { "ImageFilledRectangle",       LgdImageFilledRectangle },
    { "ImagePolygon",               LgdImagePolygon },
    { "ImageFilledPolygon",         LgdImageFilledPolygon },
    { "ImageArc",                   LgdImageArc },
    { "ImageFilledArc",             LgdImageFilledArc },
    { "ImageEllipse",               LgdImageEllipse },
    { "ImageFilledEllipse",         LgdImageFilledEllipse },
    { "ImageFill",                  LgdImageFill },
    { "ImageFillToBorder",          LgdImageFillToBorder },

    { "ImageSetAntiAliased",            LgdImageSetAntiAliased },
    { "ImageSetAntiAliasedDontBlend",   LgdImageSetAntiAliasedDontBlend },
    { "ImageSetBrush",                  LgdImageSetBrush },
    { "ImageSetTile",                   LgdImageSetTile },
    { "ImageSetStyle",                  LgdImageSetStyle },
    { "ImageSetThickness",              LgdImageSetThickness },
    { "ImageAlphaBlending",             LgdImageAlphaBlending },
    { "ImageSaveAlpha",                 LgdImageSaveAlpha },
    { "ImageGetInterlaced",             LgdImageGetInterlaced },
    { "ImageInterlace",                 LgdImageInterlace },

    { "ImageString",                LgdImageString },
    { "ImageStringUp",              LgdImageStringUp },
    { "ImageChar",                  LgdImageChar },
    { "ImageCharUp",                LgdImageCharUp },

    { "ImageCopy",                  LgdImageCopy },
    { "ImageCopyResized",           LgdImageCopyResized },
    { "ImageCopyResampled",         LgdImageCopyResampled },
    { "ImageCopyRotated",           LgdImageCopyRotated },
    { "ImageCopyMerge",             LgdImageCopyMerge },
    { "ImageCopyMergeGray",         LgdImageCopyMergeGray },
    { "ImagePaletteCopy",           LgdImagePaletteCopy },
    { "ImageSquareToCircle",        LgdImageSquareToCircle },
    { "ImageSharpen",               LgdImageSharpen },

    { NULL, NULL }
};


/* Thanks, LHF! */
static const luaL_reg LgdMetatable[] =
{
    { "__gc", LgdImageDestroy },
    { NULL, NULL }
};


int luaopen_gd(lua_State *L)
{
    luaL_openlib(L, LIB_NAME, LgdFunctions, 0);
    lua_pushliteral(L, "version");
    lua_pushliteral(L, LIB_VERSION);
    lua_settable(L, -3);

    tblseticons(L, "MaxColors", gdMaxColors);
    tblseticons(L, "GD2_FMT_RAW", GD2_FMT_RAW);
    tblseticons(L, "GD2_FMT_COMPRESSED", GD2_FMT_COMPRESSED);
    tblseticons(L, "Arc", gdArc);
    tblseticons(L, "Chord", gdChord);
    tblseticons(L, "Pie", gdPie);
    tblseticons(L, "NoFill", gdNoFill);
    tblseticons(L, "Edged", gdEdged);
    tblseticons(L, "AntiAliased", gdAntiAliased);
    tblseticons(L, "Brushed", gdBrushed);
    tblseticons(L, "Styled", gdStyled);
    tblseticons(L, "StyledBrushed", gdStyledBrushed);
    tblseticons(L, "Tiled", gdTiled);
    tblseticons(L, "Transparent", gdTransparent);

    /* Standard gd fonts */
    tblseticons(L, "FontSmall", MY_GD_FONT_SMALL);
    tblseticons(L, "FontLarge", MY_GD_FONT_LARGE);
    tblseticons(L, "FontMedium", MY_GD_FONT_MEDIUM_BOLD);
    tblseticons(L, "FontGiant", MY_GD_FONT_GIANT);
    tblseticons(L, "FontTiny", MY_GD_FONT_TINY);

    lua_pushliteral(L, "metatable");    /** metatable */
    luaL_newmetatable(L, GD_IMAGE_PTR_TYPENAME);
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -4);
    lua_settable(L, -3);
    luaL_openlib(L, NULL, LgdMetatable, 0);
    lua_settable(L, -3);

    return 0;
}

