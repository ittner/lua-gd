
-- Allows a 'require "gd"' in Lua 5.0
-- This file is not needed (and should never be used) with Lua >= 5.1.

assert(loadlib("gd", "luaopen_gd"), "Error loading gd.dll")()
