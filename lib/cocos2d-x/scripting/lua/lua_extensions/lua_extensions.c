
#include "lua_extensions.h"

#if __cplusplus
extern "C" {
#endif

// cjson
#include "cjson/lua_cjson.h"

// zlib
#include "zlib/lua_zlib.h"

// lpack
#include "lpack/lpack.h"

// socket
#if !(defined(_WP8) && defined(WP8))	// add by hanyi start here
#include "socket/luasocket.h"
#endif	// add by hanyi end here
#include "socket/mime.h"
#include "socket/socket_scripts.h"

// filesystem
#include "filesystem/lfs.h"

// lsqlite3
#include "lsqlite3/lsqlite3.h"

// xml
#include "xml/lua_xml.h"
    
// add by weixin begin
// serialize
#include "serialize/lua_cmsgpack.h"
#include "bit/lua_bit.h"
// add by weixin end

static luaL_Reg luax_exts[] = {
    {"cjson", luaopen_cjson_safe},
    {"zlib", luaopen_zlib},
    {"pack", luaopen_pack},
#if !(defined(_WP8) && defined(WP8))	// add by hanyi start here
	{ "socket.core", luaopen_socket_core },
#endif	// add by hanyi end here
	{ "mime.core", luaopen_mime_core },
    {"lfs", luaopen_lfs},
    {"lsqlite3", luaopen_lsqlite3},
	{ "xml", luaopen_LuaXML_lib },
    {"cmsgpack", luaopen_cmsgpack},
    {"bit", luaopen_bit},
    {NULL, NULL}
};

void luaopen_lua_extensions(lua_State *L)
{
    // load extensions
    luaL_Reg* lib = luax_exts;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }
    lua_pop(L, 2);

    // load extensions script
    luaopen_socket_scripts(L);
}

#if __cplusplus
} // extern "C"
#endif
