#!/usr/bin/env lua

-- $Id$

load_gd = loadlib("./libluagd.so", "luaopen_gd")        -- Unix
if load_gd == nil then
  load_gd = loadlib("./libluagd.dll", "luaopen_gd")     -- Windows
end
if load_gd == nil then
  print("Oops! Can't initialize Lua-GD; Problems when compiling?")
  os.exit(1)
end
load_gd()

function printED(res, desc)
  local str = "    " .. desc .. " "
  str = str .. string.rep(".", 37 - string.len(str))
  if res then
    print(str .. " Enabled")
  else
    print(str .. " Disabled")
  end
end

print("Lua-GD version: " .. gd.VERSION)
print("Lua-GD features:")

printED(gd.png, "PNG support")
printED(gd.gif, "GIF support")
printED(gd.jpeg, "JPEG support")
printED(gd.createFromXpm, "XPM/XBM support")
printED(gd.stringFT, "FreeType support")
printED(gd.useFontConfig, "Fontconfig support")
