load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()

im = gd.createPalette(120, 120)
assert(im)

black = im:colorAllocate(0, 0, 0)
white = im:colorAllocate(255, 255, 255)
blue = {}
for i = 1, 20 do
  blue[i] = im:colorAllocate(0, 0, 120+6*i)
end

im:gifAnimBegin("out.gif", true, 0)
for i = 1, 20 do
  tim = gd.createPalette(120, 120)
  tim:paletteCopy(im)
  tim:arc(60, 60, 6*i, 6*i, 0, 360, blue[21-i])
  tim:string(gd.FONT_MEDIUM, 120-6*2*i, 100, "Lua-GD", white)
  tim:gifAnimAdd("out.gif", false, 0, 0, 5, gd.DISPOSAL_NONE)
end

gd.gifAnimEnd("out.gif")

print("Gif animation written to file 'out.gif'.")
