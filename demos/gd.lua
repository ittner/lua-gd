-- Generic loader for Lua-GD.

-- Common locations (add new ones if needed).
local locs = {
    -- Unices
    "libluagd.so",
    "./libluagd.so",
    "/usr/lib/libluagd.so",
    "/usr/local/lib/libluagd.so",
    "/usr/local/lib/lua/5.0/lib/libluagd.so",
    -- Windows
    "libluagd.dll",
    "luagd.dll",
    ".\\luagd.dll",
    ".\\libluagd.dll",
    "c:\\lua\\lib\\libluagd.dll"
}

local loadgd, fname, ndx
for ndx, fname in ipairs(locs) do
  loadgd = loadlib(fname, "luaopen_gd")
  if loadgd then
    break
  end
end
assert(loadgd, "Can't load Lua-GD")
loadgd()
