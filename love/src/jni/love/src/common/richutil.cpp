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

#include "richutil.h"

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
			void maketableV(lua_State *L, const std::vector<std::string> &values)
			{
				lua_newtable(L); // ⁄+1
				int top = lua_gettop(L);
				int c = 1;
				for (const auto& val : values)
				{
					lua_pushinteger(L, c);
					lua_pushlstring(L, val.c_str(), val.length());
					lua_settable(L, top);
					c++; // is pain
				}
			}
			
			void maketableKV(lua_State *L, const std::map<std::string, std::string> &keyValues)
			{
				lua_newtable(L); // ⁄+1
				int top = lua_gettop(L);
				for (const auto& kv : keyValues)
				{
					lua_pushlstring(L, kv.first.c_str(), kv.first.size());
					lua_pushlstring(L, kv.second.c_str(), kv.second.size());
					lua_settable(L, top);
				}
			}
			
			void registerLoveHandler(lua_State *L, const char *name, int ref)
			{
				lua_getglobal(L, "love"); // put love on stack ⁄+1
				luaR_gettableval(L, "handlers") // put love.handlers on stack ⁄+1
				luaR_settableval(L, name, ref) // set love.handlers[name] = value(ref) ⁄±0
				lua_pop(L, 2); // clear stack ⁄-2
			}
			
			void startNewLoveThread(lua_State *L, const char luaContent[])
			{
				lua_getglobal(L, "love"); // put love on stack ⁄+1
				luaR_gettableval(L, "thread"); // get love.thread on stack ⁄+1
				
				luaR_gettableval(L, "newThread"); // get love.thread.newThread on stack ⁄+1
				lua_pushstring(L, (const char *)luaContent); // put arg on stack ⁄+1
				lua_call(L, 1, 1); // create new thread ⁄-2+1
				int threadRef = lua_ref(L, true); // register thread ⁄-1
				
				lua_getref(L, threadRef); // put thread back to stack ⁄+1
				luaR_gettableval(L, "start"); // get start function from thread ⁄+1
				lua_getref(L, threadRef); // put self to stack ⁄+1
				lua_call(L, 1, 0); // start thread ⁄-2
				
				lua_unref(L, threadRef);
				lua_pop(L, 3);
			}
			
			void debug_stackDump(lua_State *L)
			{
				printf("LUA_STACK:\t");
				int top = lua_gettop(L);
				for (int i = 1; i <= top; i++) {  /* repeat for each level */
					int t = lua_type(L, i);
					switch (t) {
							
						case LUA_TSTRING:  /* strings */
							printf("\"%s\"", lua_tostring(L, i));
							break;
							
						case LUA_TBOOLEAN:  /* booleans */
							printf(lua_toboolean(L, i) ? "true" : "false");
							break;
							
						case LUA_TNUMBER:  /* numbers */
							printf("%g", lua_tonumber(L, i));
							break;
							
						default:  /* other values */
							printf("%s", lua_typename(L, t));
							break;
							
					}
					printf("\t");  /* put a separator */
				}
				printf("\n");  /* end the listing */
			}
		}
	}
}
