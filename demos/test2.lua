load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)
white = im:ImageColorAllocate(255, 255, 255)
im:ImageRectangle(10, 10, 20, 20, white)
im:ImagePng("./out.png")
im:ImageDestroy()

os.execute("display out.png")
