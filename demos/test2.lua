load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromGif("./blerg.gif")
assert(im)
print(im:ImageColorClosest(0, 0, 0))
print(im:ImageColorsTotal())
print(im:ImageGetInterlaced())
im:ImageDestroy()
