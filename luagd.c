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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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
    {
        lua_boxpointer(L, im);
        luaL_getmetatable(L, GD_IMAGE_PTR_TYPENAME);
        lua_setmetatable(L, -2);    /* Done */
    }
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








static const luaL_reg LgdFunctions[] =
{
    { "ImageCreate",                LgdImageCreate },
    { "ImageCreatePalette",         LgdImageCreatePalette },
    { "ImageCreateTrueColor",       LgdImageCreateTrueColor },
    { "ImageDestroy",               LgdImageDestroy },
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

    { NULL, NULL }
};


int luaopen_gd(lua_State *L)
{
    luaL_openlib(L, LIB_NAME, LgdFunctions, 0);
    lua_pushliteral(L, "version");
    lua_pushliteral(L, LIB_VERSION);
    lua_settable(L, -3);

    lua_pushliteral(L, "GD2_FMT_RAW");
    lua_pushnumber(L, GD2_FMT_RAW);
    lua_settable(L, -3);

    lua_pushliteral(L, "GD2_FMT_COMPRESSED");
    lua_pushnumber(L, GD2_FMT_COMPRESSED);
    lua_settable(L, -3);

    lua_pushliteral(L, "metatable");		/** metatable */
    luaL_newmetatable(L, GD_IMAGE_PTR_TYPENAME);
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, -4);
    lua_settable(L, -3);

    return 0;
}

