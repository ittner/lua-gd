load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
gd.ImageGd(im, "./blerg.gd")
gd.ImageDestroy(im)

im = gd.ImageCreateFromGd("./blerg.gd")
gd.ImageGd2(im, "./blerg.gd2", 0, gd.GD2_FMT_RAW)
-- io.write(gd.ImageGd2Ptr(im, 0, gd.GD2_FMT_RAW))
gd.ImageDestroy(im)

im = gd.ImageCreateFromGd2("./blerg.gd2")
gd.ImagePng(im, "./blerg.png")
gd.ImageDestroy(im)



