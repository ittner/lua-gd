load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)

white = im:ImageColorAllocate(255, 255, 255)
im:ImageSetStyle({ white, white, white, white, gd.Transparent })
x, y = im:ImageSXY()
im:ImageLine(0, 0, x, y, gd.Styled)

im:ImagePng("./out.png")
im:ImageDestroy()

os.execute("display out.png")
