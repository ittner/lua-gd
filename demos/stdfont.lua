load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()

im = gd.createTrueColor(80, 110)
assert(im)

black = im:colorAllocate(0, 0, 0)
white = im:colorAllocate(255, 255, 255)

im:string(gd.FONT_TINY, 20, 20, "Tiny", white)
im:string(gd.FONT_SMALL, 20, 30, "Small", white)
im:string(gd.FONT_MEDIUM, 20, 45, "Medium", white)
im:string(gd.FONT_LARGE, 20, 58, "Large", white)
im:string(gd.FONT_GIANT, 20, 75, "Giant", white)

im:png("./out.png")
os.execute("display out.png")
