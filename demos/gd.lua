
-- Routines to find and load lua-gd.

local locs = {
    -- Unices (Linux, FreeBSD, etc.)
    "libluagd.so",          -- the default, the simple, the perfect :)
    "./libluagd.so",
    "/usr/lib/libluagd.so",
    "/usr/local/lib/libluagd.so",
    "/usr/local/lib/lua/5.0/lib/libluagd.so",
    -- Windows
    "libluagd.dll",
    "luagd.dll",
    ".\\luagd.dll",
    "c:\\lua\\lib\\libluagd.dll"
}

local loadgd = nil
local fname, _

for _,fname in ipairs(locs) do
  loadgd = loadlib(fname, "luaopen_gd")
  if loadgd then
    break
  end
end

if loadgd then
  loadgd()
else
  error("Can't load luagd")
end
