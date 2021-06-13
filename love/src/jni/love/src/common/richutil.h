/**
 * Created by Martin Braun (Marty) for love2d
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#ifndef RICHUTIL_H
#define RICHUTIL_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>

// LUA
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

namespace love
{
	namespace rich
	{
		namespace luar
		{
			void maketableV(lua_State *L, const std::vector<std::string> &values);
			void maketableKV(lua_State *L, const std::map<std::string, std::string> &keyValues);
			void registerLoveHandler(lua_State *L, const char *name, int ref);
			void startNewLoveThread(lua_State *L, const char luaContent[]);
			void debug_stackDump(lua_State *L);
		}
	}
}

#define luaR_maketableV(L, v) love::rich::luar::maketableV(L, v)
#define luaR_maketableKV(L, kv) love::rich::luar::maketableKV(L, kv)
#define luaR_regHandl(L, name, ref) love::rich::luar::registerLoveHandler(L, name, ref)
#define luaR_runCode(L, lua, name) if (luaL_loadbuffer(L, (const char *)lua, sizeof(lua), name) == 0) lua_call(L, 0, 0); else lua_error(L);
#define luaR_startThread(L, lua) love::rich::luar::startNewLoveThread(L, lua)
#define luaR_inspect(L) love::rich::luar::debug_stackDump(L)
#define luaR_gettableval(L, key) lua_pushstring(L, key); lua_gettable(L, -2);
#define luaR_settableval(L, key, vref) lua_pushstring(L, key); lua_rawgeti(L, LUA_REGISTRYINDEX, vref); lua_settable(L, -3);

#endif /* RICHUTIL_H */
