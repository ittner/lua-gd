#!/usr/bin/env lua

-- counter.lua -- a web counter in Lua!
-- (c) 2004 Alexandre Erwin Ittner

datafile="counter.txt"

require "gd"

fp = io.open(datafile, "r")
if fp then
  cnt = fp:read("*a")
  fp:close()
else
  cnt = 0
end
cnt = cnt + 1
fp = io.open(datafile, "w")
if fp then
  fp:write(cnt)
  fp:close()
end
cnt = cnt + 1

sx = string.len(cnt) * 8
if sx == 0 then
 sx = 10
end

im = gd.create(sx, 15)
-- first allocated color defines the background.
white = im:colorAllocate(255, 255, 255)
im:colorTransparent(white)
black = im:colorAllocate(0, 0, 0)
im:string(gd.FONT_MEDIUM, 1, 1, cnt, black)

print("Content-type: image/png\n")
io.write(im:pngStr())


