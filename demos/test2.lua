load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
str = gd.ImageJpegPtr(im, 75)
gd.ImageDestroy(im)

im = gd.ImageCreateFromJpegPtr(str);
im:ImageJpeg("./blerg.jpg", 75);
-- gd.ImageJpeg(im, "./blerg.jpg", 75);
gd.ImageDestroy(im)
