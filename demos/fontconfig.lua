load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()

im = gd.createTrueColor(300, 150)
white = im:colorAllocate(255, 255, 255)
black = im:colorAllocate(0, 0, 0)
x, y = im:sizeXY()
im:filledRectangle(0, 0, x, y, white)

gd.useFontConfig(true)
im:stringFT(black, "arial", 20, 0, 10, 30, "Standard Arial")
im:stringFT(black, "arial:bold", 20, 0, 10, 60, "Bold Arial")
im:stringFT(black, "arial:italic", 20, 0, 10, 90, "Italic Arial")
im:stringFT(black, "arial:bold:italic", 20, 0, 10, 120, "Italic Bold Arial")

im:png("./out.png")
os.execute("display out.png")
