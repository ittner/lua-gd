-- 
-- Plaintext Message: Meet me int the secret place at nine o'clock.
-- Encrypted Message: PyJYDpz5LCOSHPiXDvLHmVzxLV8qS7EFvZnoo1Mxk+BlT+7lMjpQKs
-- 
-- 
-- +-----+---+----------+-----------------+----------+
-- | Pix | C | Orig img |     Message     | New img  |
-- |  #  |   |   bits   | Chr | Dec | Bin |   bits   |
-- +-----+---+----------+-----+-----+-----+----------+
-- |     | R | 01010010 |     |     |  0  | 01010010 |
-- |  1  | G | 00101010 |     |     |  1  | 00101011 |
-- |_____| B | 00010101 |     |     |  0  | 00010100 |
-- |     | R | 11100100 |  P  | 080 |  1  | 11100101 |
-- |  2  | G | 00100100 |     |     |  0  | 00100100 |
-- |_____| B | 01001111 |     |     |  0  | 01001110 |
-- |     | R | 01010010 |     |     |  0  | 01010010 |
-- |  3  | G | 00101110 |_____|_____|__0__| 00101110 |
-- |_____| B | 00111001 |     |     |  0  | 00111000 |
-- |     | R | 10010110 |     |     |  1  | 10010111 |
-- |  4  | G | 01011101 |     |     |  1  | 01011101 |
-- |_____| B | 00100101 |  y  | 121 |  1  | 00100101 |
-- |     | R | 01001001 |     |     |  1  | 01001001 |
-- |  5  | G | 10110110 |     |     |  0  | 10110110 |
-- |_____| B | 00010101 |     |     |  0  | 00010100 |
-- |     | R | 00110100 |_____|_____|__1__| 00110101 |
-- |  6  | G | 01000111 |     |     |  0  | 01000110 |
-- |_____| B | 01001000 |     |     |  1  | 01001001 |
-- |     | R | 01010110 |     |     |  0  | 01010110 |
-- |  7  | G | 00011001 |     |     |  0  | 00011000 |
-- |_____| B | 10010100 |  J  | 074 |  1  | 10010101 |
-- |     | R | 00010101 |     |     |  0  | 00010100 |
-- |  8  | G | 01011010 |     |     |  1  | 01011011 |
-- |     | B | 01010001 |     |     |  0  | 01010000 |
-- +-----+---+----------+-----+-----+-----+----------+
-- 

require "gd"


function getLSB(n)
  return math.mod(n, 2) ~= 0
end


-- Stupid way to do some bit-level operations without bitlib.
function setLSB(n, b)
  if type(b) == "number" then
    if b == 0 then
      b = false
    else
      b = true
    end
  end
  if getLSB(n) then
    if b then
      return n
    elseif n > 0 then
      return n - 1
    else
      return n + 1
    end
  else
    if not b then
      return n
    elseif n > 0 then
      return n - 1
    else
      return n + 1
    end
  end
end


function intToBitArray(n)
  local ret = {}
  local i = 0
  while n ~= 0 do
    ret[i] = getLSB(n)
    n = math.floor(n/2)
    ret.size = i
    i = i + 1
  end 
  return ret
end


function printBitArray(a)
  local i
  for i = a.size,0,-1 do
    if a[i] then
      io.write("1")
    else    
      io.write("0")
    end
  end
end


function mergeMessage(im, msg)
  local w, h = im:sizeXY()
  msg = msg .. string.char(0)
  local len = string.len(msg)

  -- TRICK: if message bit length is multiple of 3*8, the code will
  -- not leave broken bit sequences.
  if math.mod(len, 3) ~= 0 then
    msg = msg .. string.rep(string.char(0), math.mod(len, 3))
    len = string.len(msg)
  end

  if h * w < len * 8 then
    return nil
  end

  local x, y = 1, 1
  local oim = gd.createTrueColor(w, h)
  local i = 1
  local a, p, c, nc, chr
  local rgb = {}

  while i <= len do
    chr = string.byte(string.sub(msg, i, i))
    a = intToBitArray(chr)
    io.write(chr .. " ")
    c = im:getPixel(x, y)
    for p = 7,0,-1 do
      if not rgb.r then
        rgb.r = setLSB(im:red(c), a[p])
      elseif not rgb.g then
        rgb.g = setLSB(im:green(c), a[p])
      else
        rgb.b = setLSB(im:blue(c), a[p])
        nc = oim:colorResolve(rgb.r, rgb.g, rgb.b)
        oim:setPixel(x, y, nc)
        x = x + 1
        if x > w then
          x = 1
          y = y + 1
        end
        rgb.r, rgb.g, rgb.b = nil, nil, nil
      end
    end
    i = i + 1
  end

  while y <= h do
    c = im:getPixel(x, y)
    nc = oim:colorResolve(im:red(c), im:green(c), im:blue(c))
    oim:setPixel(x, y, nc)
    x = x + 1
    if x > w then
      x = 1
      y = y + 1
    end
  end

  return oim
end


function getMessage(im)
  local msg = ""
  local w, h = im:sizeXY()
  local x, y = 1, 1
  local a = {}
  local s, e = 1, 1
  local b = 0
  local c

  while y <= h do
    c = im:getPixel(x, y)
    a[e] = getLSB(im:red(c))
    a[e+1] = getLSB(im:green(c))
    a[e+2] = getLSB(im:blue(c))
    e = e + 2
    if e - s >= 7 then
      b = 0
      for p = s, s+7 do
        b = b * 2
        if a[p] then
          b = b + 1
        end
        a[p] = nil
      end
      s = s + 8
      io.write(b .. " ")
      if b == 0 then
        return msg
      else
        msg = msg .. string.char(b)
      end
    end
    e = e + 1
    x = x + 1
    if x > w then
      x = 1
      y = y + 1
    end
  end

  return msg
end


function usage()
  print("Usage:")
  print("lua lsteg.lua hide <input.png> <output.png> (reads data from stdin)")
  print("lua lsteg.lua show <input.png>              (writes data to stdout)")
end



if not arg[1] or not arg[2] then
  usage()
  os.exit(1)
end

if arg[1] == "show" then
  im = gd.createFromPng(arg[2])
  if not im then    
    print("ERROR: Bad image data.")
    os.exit(1)
  end
  io.write(getMessage(im))
  os.exit(0)
end

if arg[1] == "hide" then
  if not arg[3] then
    usage()
    os.exit(1)
  end
  im = gd.createFromPng(arg[2])
  if not im then
    print("ERROR: Bad image data.")
    os.exit(1)
  end
  print("Type your message and print CTRL+D to finish.")
  msg = io.read("*a")
  oim = mergeMessage(im, msg)
  if not oim then
    print("ERROR: Image is too small for the message.")
    os.exit(1)
  end
  if not oim:png(arg[3]) then
    print("ERROR: Failed to write output file.")
    os.exit(1)
  end
  os.exit(0)
end

usage()
os.exit(1)


  



