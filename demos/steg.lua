#/usr/bin/env lua
--[[

Using Lua-GD to write steganographic messages in PNG images.
(c) 2005 Alexandre Erwin Ittner <aittner@netuno.com.br>

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.



                      Steganography with Lua-GD

Steganography is the technique of writing hidden messages in such a way
that no one apart from the intended recipient knows of the existence of
the message; this is in contrast to cryptography, where the existence
of the message is clear, but the meaning is obscured. Generally a
steganographic message will appear to be something else, like a shopping
list, an article, a picture, or some other "cover" message. In the
digital age, steganography works by replacing bits of useless or unused
data in regular computer files (such as graphics, sound, text, HTML, or
even floppy disks) with bits of different, invisible information. This
hidden information can be plain text, cipher text or even images.


                           A Simple Example

If Alice wants to send a secret message to Bob through an insecure
channel, she can use some encryption software (like GnuPG) to encrypt
the message with Bob's public key. It's a good solution because no
one unless Bob will be able to read the message. She can also sign the
message so Bob will know that the message really comes from her. BUT,
a potential attacker will know that a ciphered message was sent. If the
attacker has control over the communication channel, he can block the
message in some way that Bob will never receive it. If Alice also HIDES
the ciphertext in a unsuspected piece of information (like a photo of her
cat) the attacker will not detect it and the message will arrive to Bob.

This program will help Alice to hide some arbitrary text in a PNG image by
replacing the least significant bits of each color channel of some pixels
with bits from the encrypted message. PNG or other loseless compression
algorithm are mandatory here: if the image will be compressed by a
lossy algorithm, the hidden data can be destroyed. The maximum length
of the message are limited by the image's size (each byte needs 8 color
channels or 2 pixels and 2 channels from the next pixel). So, image must
heave at least "ceil((length+1)*8/3)" pixels (the extra byte is the NUL
marker for the end of the string). So, if Alice's message is "Meet me
in the secret place at nine o'clock.", she will encrypt and sign it to
something like "PyJYDpz5LCOSHPiXDvLHmVzxLV8qS7EFvZnoo1Mxk+BlT+7lMjpQKs"
(imagine Alice's cat walking in you keyboard :). This is the ciphertext
that will be sent to Bob through the image.

The following table shows what happens to the first eight pixels from
the image when mixed to the first three bytes from the encrypted massage:


         +-----+---+----------+-----------------+----------+
         | Pix | C | Orig img |     Message     | New img  |
         |  #  |   |   bits   | Chr | Dec | Bin |   bits   |
         +-----+---+----------+-----+-----+-----+----------+
         |     | R | 01010010 |     |     |  0  | 01010010 |
         |  1  | G | 00101010 |     |     |  1  | 00101011 |
         |_____| B | 00010101 |     |     |  0  | 00010100 |
         |     | R | 11100100 |  P  | 080 |  1  | 11100101 |
         |  2  | G | 00100100 |     |     |  0  | 00100100 |
         |_____| B | 01001111 |     |     |  0  | 01001110 |
         |     | R | 01010010 |     |     |  0  | 01010010 |
         |  3  | G | 00101110 |_____|_____|__0__| 00101110 |
         |_____| B | 00111001 |     |     |  0  | 00111000 |
         |     | R | 10010110 |     |     |  1  | 10010111 |
         |  4  | G | 01011101 |     |     |  1  | 01011101 |
         |_____| B | 00100101 |  y  | 121 |  1  | 00100101 |
         |     | R | 01001001 |     |     |  1  | 01001001 |
         |  5  | G | 10110110 |     |     |  0  | 10110110 |
         |_____| B | 00010101 |     |     |  0  | 00010100 |
         |     | R | 00110100 |_____|_____|__1__| 00110101 |
         |  6  | G | 01000111 |     |     |  0  | 01000110 |
         |_____| B | 01001000 |     |     |  1  | 01001001 |
         |     | R | 01010110 |     |     |  0  | 01010110 |
         |  7  | G | 00011001 |     |     |  0  | 00011000 |
         |_____| B | 10010100 |  J  | 074 |  1  | 10010101 |
         |     | R | 00010101 |     |     |  0  | 00010100 |
         |  8  | G | 01011010 |     |     |  1  | 01011011 |
         |     | B | 01010001 |     |     |  0  | 01010000 |
         +-----+---+----------+-----+-----+-----+----------+


When Bob wants to read the message he will extract the least significant
bit (LSB) from each color channel from some pixels of the image and
join them to get the original ciphertext. A NULL character (ASCII #0)
will mark the end of the message within the image, so will know when
stop. Of course, this program will also do this boring job for Bob.


$Id$

--]]


require "gd"


function getLSB(n)
  return math.mod(n, 2) ~= 0
end


-- Bizarre way to do some bit-level operations without bitlib.
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
  -- not leave "broken" bit sequences.
  if math.mod(len, 3) ~= 0 then
    msg = msg .. string.rep(string.char(0), math.mod(len, 3))
    len = string.len(msg)
  end
  if h * w < len * 8 then
    return nil
  end
  local x, y = 0, 0
  local oim = gd.createTrueColor(w, h)
  local i = 1
  local a2, c, nc, chr
  local a = {}
  local s, e = 1, 1
  local rgb = {}

  while y < h do
    c = im:getPixel(x, y)
    rgb.r = im:red(c)
    rgb.g = im:green(c)
    rgb.b = im:blue(c)
    if i < len and  e - s < 3 then
      a2 = intToBitArray(string.byte(string.sub(msg, i, i)))
      for cnt = 7,0,-1 do
        a[e+7-cnt] = a2[cnt]
      end
      i = i + 1
      e = e + 8
    end
    if e - s > 0 then
      rgb.r = setLSB(rgb.r, a[s])
      a[s] = nil
      s = s + 1
    end
    if e - s > 0 then
      rgb.g = setLSB(rgb.g, a[s])
      a[s] = nil
      s = s + 1
    end
    if e - s > 0 then
      rgb.b = setLSB(rgb.b, a[s])
      a[s] = nil
      s = s + 1
    end
    nc = oim:colorResolve(rgb.r, rgb.g, rgb.b)
    oim:setPixel(x, y, nc)
    x = x + 1
    if x == w then
      x = 0
      y = y + 1
    end
  end

  return oim, (i-1)*8, w*h
end


function getMessage(im)
  local msg = ""
  local w, h = im:sizeXY()
  local x, y = 0, 0
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
      if b == 0 then
        return msg
      else
        msg = msg .. string.char(b)
      end
    end
    e = e + 1
    x = x + 1
    if x == w then
      x = 0
      y = y + 1
    end
  end

  return msg
end


function usage()
  print("Usage:")
  print("  lua steg.lua hide input.png output.png   (reads data from stdin)")
  print("  lua steg.lua show input.png              (writes data to stdout)")
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
  print("Type your message and press CTRL+D to finish.")
  msg = io.read("*a")
  oim, l, t = mergeMessage(im, msg)
  if not oim then
    print("ERROR: Image is too small for the message.")
    os.exit(1)
  end
  if not oim:png(arg[3]) then
    print("ERROR: Failed to write output file.")
    os.exit(1)
  end
  print(string.format("DONE: %2.2f%% of the image used to store the message.",
    l/t*100.0))
  os.exit(0)
end

usage()
os.exit(1)

