load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

print(gd.version)

im = gd.ImageCreateFromJpeg("./bugs.jpg")
if gd.ImageJpeg(im, "out.jpg", 75) then
  print("Done")
else
  print("Error")
end
gd.ImageDestroy(im)
