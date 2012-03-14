-- This is an unofficial modified version of the 2.0.2-5 rockspec that replaces
-- the Windows build system so that it actually works.

package = "LuaSocket"
version = "2.0.2-5"
source = {
   url = "http://luaforge.net/frs/download.php/2664/luasocket-2.0.2.tar.gz",
   md5 = "41445b138deb7bcfe97bff957503da8e"
}
description = {
   summary = "Network support for the Lua language",
   detailed = [[
      LuaSocket is a Lua extension library that is composed by two parts: a C core
      that provides support for the TCP and UDP transport layers, and a set of Lua
      modules that add support for functionality commonly needed by applications
      that deal with the Internet.
   ]],
   homepage = "http://luaforge.net/projects/luasocket/",
   license = "MIT"
}
dependencies = {
   "lua >= 5.1, < 5.2"
}
build = {
   type = "make",
   build_variables = {
      CFLAGS = "$(CFLAGS) -DLUASOCKET_DEBUG -I$(LUA_INCDIR)",
      LDFLAGS = "$(LIBFLAG) -O -fpic",
      LD = "$(CC)"
   },
   install_variables = {
      INSTALL_TOP_SHARE = "$(LUADIR)",
      INSTALL_TOP_LIB = "$(LIBDIR)"
   },
   platforms = {
      macosx = {
         build_variables = {
            CFLAGS = "$(CFLAGS) -DLUASOCKET_DEBUG -DUNIX_HAS_SUN_LEN -fno-common -I$(LUA_INCDIR)"
         }
      },
      windows={
		type="builtin",
		modules = {
			["ltn12"] = "src/ltn12.lua",
			mime = "src/mime.lua",
			socket = "src/socket.lua",		
			["socket.ftp"] = "src/ftp.lua",
			["socket.http"] = "src/http.lua",
			["socket.smtp"] = "src/smtp.lua",
			["socket.tp"] = "src/tp.lua",
			["socket.url"] = "src/url.lua",  
			["mime.core"] = {
				sources = {"src/mime.c"},
				defines = {"MIME_EXPORTS", "MIME_API=__declspec(dllexport)"}
			},
			["socket.core"] = {
				sources = {"src/auxiliar.c", "src/buffer.c", "src/except.c", "src/inet.c", "src/io.c", "src/luasocket.c", "src/options.c", "src/select.c", "src/tcp.c", "src/timeout.c", "src/udp.c", "src/wsocket.c"},
				defines = {"LUASOCKET_EXPORTS", "LUASOCKET_API=__declspec(dllexport)"},
				libraries = {"ws2_32"}
			}
		}
      }
   },
   copy_directories = { "doc", "samples", "etc", "test" }
}
