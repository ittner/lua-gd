-- Easy way to put  a library on the path.
-- (c) 2005 Alexandre Erwin Ittner 
--
--  print(string.gsub("aittnerXnetunoYcomYbr", "[XY]",
--     function(x) return ({ X = '@', Y = '.'})[x] end))
--
--


-- Splits an package.cpath into a table.
function splitpath(str)
  local t = {}
  local v
  for v in string.gfind(str, "([^;]+);") do
    table.insert(t, v)
  end
  return t
end

-- Finds the first absolute path on the list. Works with *nix (MacOS X
-- included) and Windows paths.
function findplace(tbl)
  local k, v
  for k, v in ipairs(tbl) do
    if v:sub(1, 1) == "/"
    or v:sub(2, 3) == ":\\" then
      return v
    end
  end
end

function copy(fname, name)
  local tpath = splitpath(package.cpath)
  local path = findplace(tpath)
  assert(path, "Can't find a place to install")
  local dest = path:gsub("%?", name)
  print(dest)
end


copy(arg[1], arg[2])
