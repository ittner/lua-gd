load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)
white = im:ImageColorAllocate(255, 255, 255)

im:ImageArc(100, 100, 200, 200,  0, 180, white)
im:ImageArc(100, 100, 180, 180, 20, 160, white)
im:ImageArc(100, 100, 160, 160, 40, 140, white)
im:ImageArc(100, 100, 140, 140, 60, 120, white)
im:ImageArc(100, 100, 120, 120, 80, 100, white)

im:ImageFilledArc(50, 50, 80, 80, 60, 120, white, gd.Arc)

im:ImageFilledEllipse(200, 200, 20, 20, white)

im:ImagePng("./out.png")
im:ImageDestroy()

os.execute("display out.png")
