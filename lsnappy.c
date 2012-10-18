/*
 * lua-csnappy : <http://fperrad.github.com/lua-csnappy/>
 */

#include "lua.h"
#include "lauxlib.h"

#include "csnappy/csnappy_compress.c"
#include "csnappy/csnappy_decompress.c"

static int lsnappy_compress(lua_State *L)
{
    void* ud;
    size_t src_len;
    size_t dest_len;
    const char* src = luaL_checklstring(L, 1, &src_len);
    size_t dest_max = csnappy_max_compressed_length(src_len);
    lua_Alloc alloc = lua_getallocf(L, &ud);
    char* working_memory = alloc(ud, NULL, 0, CSNAPPY_WORKMEM_BYTES);
    char* dest = alloc(ud, NULL, 0, dest_max);
    csnappy_compress(src, src_len, dest, &dest_len, working_memory,
                     CSNAPPY_WORKMEM_BYTES_POWER_OF_TWO);
    lua_pushlstring(L, dest, dest_len);
    /* freeing */
    (void)alloc(ud, dest, dest_max, 0);
    (void)alloc(ud, working_memory, CSNAPPY_WORKMEM_BYTES, 0);
    return 1;
}

static int lsnappy_decompress(lua_State *L)
{
    void* ud;
    size_t src_len;
    size_t dest_len;
    const char* src = luaL_checklstring(L, 1, &src_len);
    size_t header_len = csnappy_get_uncompressed_length(src, src_len, &dest_len);
    lua_Alloc alloc = lua_getallocf(L, &ud);
    char* dest = alloc(ud, NULL, 0, dest_len);
    csnappy_decompress_noheader(src + header_len, src_len - header_len, dest, &dest_len);
    lua_pushlstring(L, dest, dest_len);
    /* freeing */
    (void)alloc(ud, dest, dest_len, 0);
    return 1;
}


static const luaL_reg snappy[] =
{
    { "compress",       lsnappy_compress        },
    { "decompress",     lsnappy_decompress      },
    { NULL, NULL }
};

LUALIB_API int luaopen_snappy(lua_State *L)
{
    luaL_register(L, "snappy", snappy);
    lua_pushliteral (L, "Copyright (C) 2012 Francois Perrad");
    lua_setfield(L, -2, "_COPYRIGHT");
    lua_pushliteral (L, "lua-csnappy: a fast compressor/decompressor");
    lua_setfield(L, -2, "_DESCRIPTION");
    lua_pushliteral (L, "0.1.0");
    lua_setfield(L, -2, "_VERSION");
    return 1;
}
