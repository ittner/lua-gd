load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)

white = im:ImageColorAllocate(255, 255, 255)
gray = im:ImageColorAllocate(128, 128, 128)
x, y = im:ImageSXY()

im:ImageString(gd.FontMedium, 10, 10, "Powered by Lua-GD", white)

print(im:ImageStringFT(gray, "./tolkien.ttf", 16, 0, 150, 150, "Teste"))


im:ImagePng("./out.png")
os.execute("display out.png")
