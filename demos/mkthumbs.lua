#!/usr/bin/env lua

-- Thumbnail maker with lua and lua-gd
-- (c) 2004 Alexandre Erwin Ittner <aittner@netuno.com.br>
-- Distributed under the terms of GNU GPL, version 2 or (at your option) any
-- later version.  THERE IS NO WARRANTY.

-- This program runs under Unix only and requires the Luiz Henrique de
-- Figueiredo's POSIX extension for ua, which can be donwloaded from
-- http://www.tecgraf.puc-rio.br/~lhf/ftp/lua/

-- $Id$


if arg[1] == nil then
  print("usage:  mkthumbs.lua <directory>")
  os.exit(1)
end

load_posix = loadlib("lposix.so", "luaopen_posix")
if load_posix == nil then
  print("Error:  Can't find the POSIX library. Do you have it, no?")
  os.exit(1)
end
load_posix()

