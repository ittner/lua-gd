load_gd = assert(loadlib("./libluagd.so", "luaopen_gd"))
load_gd()

im = gd.ImageCreateFromJpeg("./bugs.jpg")
assert(im)
white = im:ImageColorAllocate(255, 255, 255)
path =  {
    {  5, 10 },
    { 10, 30 },
    { 20, 35 },
    { 30, 30 },
    { 35, 10 },
    { 30,  5 },
    { 15,  5 }
}

im:ImagePolygon(path, white) 

im:ImagePng("./out.png")
im:ImageDestroy()

os.execute("display out.png")
