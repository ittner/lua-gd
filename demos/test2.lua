load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
io.write(gd.ImageJpegPtr(im, 75))
gd.ImageDestroy(im)

