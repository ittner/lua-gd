#!/usr/bin/env lua

load_gd = loadlib("./libluagd.so", "luaopen_gd")        -- Unix
if load_gd == nil then
  load_gd = loadlib("./libluagd.dll", "luaopen_gd")     -- Windows
end
if load_gd == nil then
  print("Oops! Can't initialize Lua-GD; Problems when compiling?")
  os.exit(1)
end
load_gd()

print("Lua-GD version: " .. gd.VERSION)
print("Lua-GD features:")

if gd.png then
  print("PNG support:           Enabled")
else
  print("PNG support:           Disabled")
end

if gd.gif then
  print("GIF support:           Enabled")
else
  print("GIF support:           Disabled")
end

if gd.jpeg then
  print("JPEG support:          Enabled")
else
  print("JPEG support:          Disabled")
end

if gd.createFromXpm then
  print("XPM/XBM support:       Enabled")
else
  print("XPM/XBM support:       Disabled")
end

if gd.png then
  print("PNG support:           Enabled")
else
  print("PNG support:           Disabled")
end

if gd.stringFT then
  print("FreeType support:      Enabled")
else
  print("FreeType support:      Disabled")
end

if gd.useFontConfig then
  print("Fontconfig support:    Enabled")
else
  print("Fontconfig support:    Disabled")
end


