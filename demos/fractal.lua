#!/usr/bin/env lua

load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()

size = 500

im = gd.createPalette(size, size)
white = im:colorAllocate(255, 255, 255)
black = im:colorAllocate(0, 0, 0)

m = {}
m[math.floor(size/2)] = true

for i = 1, size do
  n = {}
  for j = 1, size do
    if m[j] then
      im:setPixel(j, i, black)
      n[j+1] = not n[j+1]
      n[j-1] = not n[j-1]
    else
      im:setPixel(j, i, white)
    end
  end
  m = n
end

im:png("./out.png")
os.execute("display out.png")

