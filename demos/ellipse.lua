load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()

im = gd.createTrueColor(80, 80)
assert(im)

black = im:colorAllocate(0, 0, 0)
white = im:colorAllocate(255, 255, 255)
im:filledEllipse(40, 40, 70, 50, white)

im:png("./out.png")
os.execute("display out.png")
