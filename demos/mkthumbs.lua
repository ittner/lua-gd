#!/usr/bin/env lua

-- Thumbnail maker with lua and lua-gd
-- (c) 2004 Alexandre Erwin Ittner <aittner@netuno.com.br>
-- Distributed under the terms of GNU GPL, version 2 or (at your option) any
-- later version.  THERE IS NO WARRANTY.

-- This program runs under Unix only and requires the Luiz Henrique de
-- Figueiredo's POSIX extension for ua, which can be donwloaded from
-- http://www.tecgraf.puc-rio.br/~lhf/ftp/lua/

-- $Id$


thumbsize = 80          -- thumbnail size

header = [[
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
 <head>
  <title>Thumbnails for {DIRNAME}</title>
  <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
  <style type="text/css">
<!--
H1 {
    font-family: Verdana, Arial, helvetica, sans-serif;
    font-size: 18px;
    color: black;
    background-color: white;
    text-align: center;
}    

BODY  {
    font-family: Verdana, Arial, helvetica, sans-serif;
    font-size: 11px;
    color: black;
    background : white;
}

TABLE {
    border: 0px;
}

TR, TD {
    font-family: Verdana, Arial, helvetica, sans-serif;
    font-size: 11px;
    border: 0px;
    background-color: white;
    padding: 0px;
    cell-spacing: 1px;
}
-->
  </style>
 </head>

 <body>
  <h1>Thumbnails for {DIRNAME}</h1>
   <table>
]]

footer = [[
  </table>
 </body>
</html>
]]



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

load_gd = assert(loadlib("libluagd.so", "luaopen_gd"))
load_gd()



function makeThumb(fname)
  local im
  local tmpname = string.lower(fname)
  local s, e, name
  local thumbname
  local format

  s, e, name = string.find(tmpname, "(.+)%.png")
  if name then
    im = gd.createFromPng(fname)
    format = "PNG"
  end
  s, e, name = string.find(tmpname, "(.+)%.jpe?g")
  if name then
    im = gd.createFromJpeg(fname)
    format = "JPEG"
  end
  s, e, name = string.find(tmpname, "(.+)%.gif")
  if name then
    im = gd.createFromGif(fname)
    format = "GIF"
  end
  if im == nil then
    print("Error: " .. fname .. " unsuported format")
  end

  thumbname = name .. ".png"
  local sx, sy = im:sizeXY()
  local tsy, tsy

  if sx <= thumbsize and sy <= thumbsize then
    tsx, tsy = sx, sy
  else
    local factor
    factor = math.max(1, sx/thumbsize, sy/thumbsize)
    txs, tsy = sx/factor, sy/factor
  end

  tim = gd.CreateTrueColor(tsx, tsy+10)

end





dirname = arg[1]
filelist = posix.dir(dirname)



