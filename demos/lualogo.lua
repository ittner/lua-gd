
require "gd"

function makelogo(size)
  local im = gd.createTrueColor(size, size)
  local white = im:colorAllocate(255, 255, 255)
  local blue = im:colorAllocate(0, 0, 128)
  local gray = im:colorAllocate(140, 140, 140)

  local ediam = size * 0.68                     -- Earth diameter
  local mdiam = ediam * (1 - math.sqrt(2) / 2)  -- Moon diameter
  local odiam = ediam * 1.3                     -- Orbit diameter
  local emdist = odiam/2 * 1.05                 -- Earth - Moon distance
  local esdist = odiam/2 * 0.4                  -- Earth - Moon shadow distance
  local mang = 45                               -- Moon angle (degrees)
  local mangr = math.rad(mang)
  local cxy = size/2

  im:fill(0, 0, white)
  im:filledArc(cxy, cxy, ediam, ediam, 0, 360, blue, gd.ARC)

  im:setThickness(math.max(0.03 * ediam, 1))
  local i
  for i = 0, 360, 10 do
    im:arc(cxy, cxy, odiam, odiam, i, i+5, gray)
  end
  im:setThickness(1)

  -- Moon
  local mcx = cxy + math.sin(math.rad(mang)) * emdist
  local mcy = cxy - math.cos(math.rad(mang)) * emdist
  im:filledArc(mcx, mcy, mdiam, mdiam, 0, 360, blue, gd.ARC)

  -- Moon shadow
  local mscx = cxy + math.sin(math.rad(mang)) * esdist
  local mscy = cxy - math.cos(math.rad(mang)) * esdist
  im:filledArc(mscx, mscy, mdiam, mdiam, 0, 360, white, gd.ARC)

  return im 
end


makelogo(140):png("lualogo.png")
os.execute("display lualogo.png")

