load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)

white = im:ImageColorAllocate(255, 255, 255)
im:ImageString(gd.FontMedium, 10, 10, "Powered by", white)

imlua = gd.ImageCreateFromPng("./lua-gd.png")
-- imlua:ImageColorTransparent(imlua:ImageGetPixel(0, 0))

sx, sy = imlua:ImageSXY()
gd.ImageCopy(im, imlua, 10, 25, 0, 0, sx, sy, sx, sy)

im:ImagePng("./out.png")
os.execute("display out.png")
