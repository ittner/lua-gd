package = "LuaGD"
version = "2.0.33r2-1"

source = {
    -- url = "http://luaforge.net/frs/download.php/1592/lua-gd-2.0.33r2.tar.gz",
    url = "http://www.ittner.com.br/lua/lua-gd-2.0.33r2.tar.gz",
    md5 = "69e9c07ce208ce13d4373177c2e24df0"
}

description = {
   summary = "Lua binding to LibGD",
   detailed = [[
Lua-GD is a set of Lua bindings to the Thomas Boutell's gd library that
allows your code to quickly draw complete images with lines, polygons, arcs,
text, multiple colors, cut and paste from other images, flood fills, read in
or write out images in the PNG, JPEG or GIF format. It is not a kitchen-sink
graphics package, but it does include most frequently requested features,
including both truecolor and palette images, resampling (smooth resizing of
truecolor images) and so forth. It is particularly useful in Web applications.
]],
    homepage = "http://lua-gd.luaforge.net/",
    license = "MIT/X11",
    maintainer = "Alexandre Erwin Ittner"
}

dependencies = {
    "lua >= 5.1"
}

external_dependencies = {
    GD = { header = "gd.h" }
}

build = {
    type = "make",
    platforms = {
        unix = {
            build_pass = true,
            install_pass = false,
            install = { lib = { "gd.so" } },
            copy_directories = { "doc", "demos" }
        }
        -- Some way to detect GD features on Windows?
    }
}
