load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
io.write(gd.ImagePngPtrEx(im, 9))
gd.ImageDestroy(im)

