load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)
white = im:ImageColorAllocate(255, 255, 255)
im:ImageLine(0, 0, 20, 20, white)
im:ImageSetPixel(25, 25, white)

at30 = im:ImageGetPixel(30, 30)
print(at30)
if at30 then
  im:ImageLine(0, 20, 20, 0, at30)
end

im:ImagePng("./out.png")
im:ImageDestroy()

os.execute("display out.png")
