load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateTrueColor(300, 50)
white = im:ImageColorAllocate(255, 255, 255)
black = im:ImageColorAllocate(0, 0, 0)
x, y = im:ImageSXY()
im:ImageFilledRectangle(0, 0, x, y, white)
im:ImageStringFT(black, "./tolkien.ttf", 20, 0, 10, 30, "One Ring to rule them all")
im:ImagePng("./out.png")

os.execute("display out.png")
