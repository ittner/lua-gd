require "gd"

x, y = 140, 110

im = assert(gd.createTrueColor(x, y))
white = im:colorAllocate(255, 255, 255)
black = im:colorAllocate(0, 0, 0)
im:filledRectangle(0, 0, x, y, white)

im:string(gd.FONT_TINY, 10, 20, "gd.FONT_TINY", black)
im:string(gd.FONT_SMALL, 10, 30, "gd.FONT_SMALL", black)
im:string(gd.FONT_MEDIUM, 10, 45, "gd.FONT_MEDIUM", black)
im:string(gd.FONT_LARGE, 10, 58, "gd.FONT_LARGE", black)
im:string(gd.FONT_GIANT, 10, 75, "gd.FONT_GIANT", black)

im:png("out.png")
os.execute("display out.png")
