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

  local ang = 0
  while ang <= 360 do
    im:line(
      size/2 + 0.9 * size * math.sin(math.rad(ang)),
      size - (size/2 + 0.9 * size * math.cos(math.rad(ang))),
      size/2 + 0.95 * size * math.sin(math.rad(ang)),
      size - (size/2 + 0.95 * size * math.cos(math.rad(ang))),
      gray)
    ang = ang + 30
  end

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


s = 128
im = createClock(s, "Brasil", 10, 10)
im:png("./out.png")
os.execute("display out.png")

