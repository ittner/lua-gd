load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()

im = gd.createTrueColor(300, 50)
white = im:colorAllocate(255, 255, 255)
black = im:colorAllocate(0, 0, 0)
x, y = im:sizeXY()
im:filledRectangle(0, 0, x, y, white)
im:stringFT(black, "./tolkien.ttf", 20, 0, 10, 30, "One Ring to rule them all")
im:png("./out.png")

os.execute("display out.png")
