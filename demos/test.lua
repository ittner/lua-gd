load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

math.randomseed(os.time())

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)
sx, sy = im:ImageSXY()

im2 = gd.ImageCreateTrueColor(2*sx, sy)
black = im2:ImageColorAllocate(0, 0, 0)
white = im2:ImageColorAllocate(255, 255, 255)
gd.ImageCopy(im2, im, 0, 0, 0, 0, sx, sy, sx, sy)

sx2, sy2 = im2:ImageSXY()
im2:ImageStringUp(gd.FontSmall, 5, sy2-10, gd.version, white)

for i = 0, 14 do
  for j = 0, 24 do
    rcl = im2:ImageColorAllocate(math.random(255), math.random(255),
                    math.random(255))
    im2:ImageFilledRectangle(sx+20+j*10, i*20+40, sx+30+j*10, i*20+50, rcl)
  end
end

im2:ImageString(gd.FontGiant, sx+80, 10, "Powered by Lua", white)

blackTr = im2:ImageColorAllocateAlpha(0, 0, 0, 80)
im2:ImageStringFT(blackTr, "./tolkien.ttf", 140, 0, 70, 130, "gd")
im2:ImageStringFT(white, "./tolkien.ttf", 50, math.pi/5, 340, 250, "FreeType")


im2:ImagePng("./out.png")
os.execute("display out.png")
