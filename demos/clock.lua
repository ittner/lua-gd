load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()


function createClock(size, title, hours, minutes)
  local im = gd.createTrueColor(size, size)
  local white = im:colorAllocate(255, 255, 255)
  local gray = im:colorAllocate(128, 128, 128)
  local black = im:colorAllocate(0, 0, 0)
  local blue = im:colorAllocate(0, 0, 128)
  local cxy = size/2

  im:filledRectangle(0, 0, size, size, white)
  im:setThickness(math.max(1, size/100))
  im:arc(cxy, cxy, size, size, 0, 360, gray)

  im:line(size/2, 0, size/2, 0.1*size, gray)
  im:line(0, size/2, 0.1*size, size/2, gray)
  im:line(size/2, size, size/2, 0.9*size, gray)
  im:line(0.9*size, size/2, size, size/2, gray)

  
  im:string(gd.FONT_SMALL, cxy - string.len(title) * 2.5, 0.6 * size, title, blue)  

  im:setThickness(math.max(1, size/50))
  im:line(size/2, size/2,
    size/2 + 0.45 * size * math.sin(math.rad(6*minutes)),
    size - (size/2 + 0.45 * size * math.cos(math.rad(6*minutes))),
    blue)

  im:setThickness(math.max(1, size/25))
  im:line(size/2, size/2, 
    size/2 + 0.25 * size * math.sin(math.rad(30*hours)),
    size - (size/2 + 0.25 * size * math.cos(math.rad(30*hours))),
    blue)

  im:setThickness(1)
  local sp = math.max(1, size/20)
  im:filledArc(cxy, cxy, sp, sp, 0, 360, gray, gd.ARC)

  return im
end


s = 120
im = createClock(s, "Brasil", 10, 10)
im:png("./out.png")
os.execute("display out.png")

