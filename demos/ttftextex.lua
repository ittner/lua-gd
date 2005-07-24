require "gd"

im = gd.createTrueColor(400, 400)
assert(im)

black = im:colorAllocate(0, 0, 0)
grayt = im:colorAllocateAlpha(255, 255, 255, 70)
bluet = im:colorAllocateAlpha(0, 0, 250, 70)
redt = im:colorAllocateAlpha(255, 0, 0, 0)
greent = im:colorAllocateAlpha(0, 250, 0, 70)
lbluet = im:colorAllocateAlpha(180, 180, 255, 70)
yellowt = im:colorAllocateAlpha(240, 240, 0, 70)

im:stringFTEx(redt, "Vera.ttf", 20, 0, 40, 40, "Single\nspace", 
    { linespacing = 1.0 } )

im:stringFTEx(greent, "Vera.ttf", 20, 0, 40, 150, "Double\nspace", 
    { linespacing = 2.0 } )

im:png("out.png")
os.execute("display out.png")
