load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()


function boxedtext(im, color, size, ang, x, y, text, bcolor)
  llx, lly, lrx, lry, urx, ury, ulx, uly =
    im:stringFT(color, "./Vera.ttf", size, math.rad(ang), x, y, text)
  im:polygon({ {llx, lly}, {lrx, lry}, {urx, ury}, {ulx, uly} }, bcolor)
end

im = gd.createTrueColor(400, 400)
assert(im)

black = im:colorAllocate(0, 0, 0)
grayt = im:colorAllocateAlpha(255, 255, 255, 70)
bluet = im:colorAllocateAlpha(0, 0, 250, 70)
redt = im:colorAllocateAlpha(255, 0, 0, 0)
greent = im:colorAllocateAlpha(0, 250, 0, 70)
lbluet = im:colorAllocateAlpha(180, 180, 255, 70)
yellowt = im:colorAllocateAlpha(240, 240, 0, 70)

boxedtext(im, yellowt, 300, 0, 60, 350, "A", bluet)
boxedtext(im, greent, 80, 45, 60, 220, "Ithil", bluet)
boxedtext(im, redt, 45, 90, 380, 300, "Lua-GD", bluet)
boxedtext(im, lbluet, 36, 290, 160, 130, "FreeType", bluet)
boxedtext(im, grayt, 26, 180, 390, 360, "Turn 180° before read", bluet)

im:png("./out.png")
os.execute("display out.png")
