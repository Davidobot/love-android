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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <map>
#include <common/delay.h>

// LOVE
#include "wrap_MobSvc.h"
#include "sdl/MobSvc.h"

// LUA
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

// RICH
#include "common/richutil.h"

namespace love
{
	namespace mobsvc
	{

#define instance() (Module::getInstance<MobSvc>(Module::M_MOBSVC))

		const char *moduleName = "mobsvc";
		long fnc = 0;
		char formatBuffer[1024];
		const char *handlClr = "mobSvcClearCallbacks";

		int w_test(lua_State *L __unused)
		{
			instance()->test();
			return 0;
		}

		int w_signIn(lua_State *L)
		{
			int na = 1, ao = lua_gettop(L) - na, type, ref;
			if(ao > 0) lua_pop(L, ao); // throw unnecessary arguments out of the stack to prevent issues

			type = lua_type(L, -1);
            const char *loveCallback = "mobileServiceSignedIn";
            const char *handlCallback = "mobSvcSignInCallback";
            const char *handlStaticCallback = "mobSvcSignInStaticCallback";
            // the fnc approach fails to set the handlers at 1, some questions in the universe will always struggle humanity ...

			if (type == LUA_TFUNCTION) { // arg: callback
				ref = lua_ref(L, true); // pop arg from stack into registry ⁄-1
                luaR_regHandl(L, handlCallback, ref); // love.handlers[handlCallback] = value(ref) ⁄±0
                lua_unref(L, ref); // remove arg from registry
				ao++; // put nil later on stack ⁄+1
			}

			lua_getglobal(L, "love"); // put love on stack ⁄+1
			luaR_gettableval(L, loveCallback); // put love.mobileServiceSignedIn on stack ⁄+1
			ref = lua_ref(L, true); // pop love.mobileServiceSignedIn from stack into registry ⁄-1
			lua_pop(L, 1); // clear stack ⁄-1
			luaR_regHandl(L, handlStaticCallback, ref); // love.handlers[handlStaticCallback] = value(ref) ⁄±0
			lua_unref(L, ref); // remove love.mobileServiceSignedIn from registry

			// prepare callback clear handler and callback fallback handlers
            sprintf(formatBuffer, R"luastring"--(
				local handlClr, __NULL__ = love['%s'].__clearCallbacks, love['%s'].__NULL__
				love.handlers['%s'] = handlClr
				if not rawget(love.handlers, '%s') then love.handlers['%s'] = __NULL__ end
				if not rawget(love.handlers, '%s') then love.handlers['%s'] = __NULL__ end
			--)luastring"--", moduleName, moduleName, handlClr, handlCallback, handlCallback, handlStaticCallback, handlStaticCallback);
            luaR_runCode(L, formatBuffer, "mobsvc.w_signIn");
            memset(formatBuffer, 0, sizeof formatBuffer);

			// call await function in new LÖVE thread
			sprintf(formatBuffer, R"luastring"--(
				require('love.event') require('love.%s') local e, this = love.event, love['%s']
				local playerId = this.signInAwait()
				if playerId then e.push("%s", playerId) e.push("%s", playerId) end
				e.push("%s", { "%s", "%s" })
			--)luastring"--", moduleName, moduleName, handlCallback, handlStaticCallback, handlClr, handlCallback, handlStaticCallback);
			luaR_startThread(L, formatBuffer);
			memset(formatBuffer, 0, sizeof formatBuffer);

			for(int i = 0; i < ao; i++) lua_pushnil(L); // reset stack
			return 0;
		}

		int w_signInAwait(lua_State *L)
		{
			std::string ret = instance()->signInAwait();
			if(!ret.empty())
			{
				luax_pushstring(L, ret);
                return 1;
			}
			return 0;
		}

		int w_isSignedIn(lua_State *L)
		{
			luax_pushboolean(L, instance()->isSignedIn());
			return 1;
		}

        int w_showAchievements(lua_State *L __unused)
        {
            instance()->showAchievements();
            return 0;
        }

        int w_incrementAchievementProgress(lua_State *L)
        {
            int na = 5, ao = lua_gettop(L) - na, type, ref;
            if(ao > 0) lua_pop(L, ao); // throw unnecessary arguments out of the stack to prevent issues

			const char *androidAchievementId = luaL_checkstring(L, 1);
			const char *iosAchievementId = luaL_checkstring(L, 2);
			int steps = luaL_checkint(L, 3);
			int maxSteps = luaL_checkint(L, 4);

            type = lua_type(L, -1);
            fnc += 1; // prepare unique module call number as string
            const char *loveCallback = "mobileServiceAchievementProgressIncremented";
            char handlCallback[64]; sprintf(handlCallback, "mobSvcIncrementAchievementProgressCallback_%ld", fnc);
            char handlStaticCallback[64]; sprintf(handlStaticCallback, "mobSvcIncrementAchievementProgressStaticCallback_%ld", fnc);

            if (type == LUA_TFUNCTION) { // arg: callback
                ref = lua_ref(L, true); // pop arg from stack into registry ⁄-1
                luaR_regHandl(L, handlCallback, ref); // love.handlers[handlCallback] = value(ref) ⁄±0
                lua_unref(L, ref); // remove arg from registry
                ao++; // put nil later on stack ⁄+1
            }

            lua_getglobal(L, "love"); // put love on stack ⁄+1
            luaR_gettableval(L, loveCallback); // put love.mobileServiceSignedIn on stack ⁄+1
            ref = lua_ref(L, true); // pop love.mobileServiceSignedIn from stack into registry ⁄-1
            lua_pop(L, 1); // clear stack ⁄-1
            luaR_regHandl(L, handlStaticCallback, ref); // love.handlers[handlStaticCallback] = value(ref) ⁄±0
            lua_unref(L, ref); // remove love.mobileServiceSignedIn from registry

            // prepare callback clear handler and callback fallback handlers
            sprintf(formatBuffer, R"luastring"--(
				local handlClr, __NULL__ = love['%s'].__clearCallbacks, love['%s'].__NULL__
				love.handlers['%s'] = handlClr
				if not rawget(love.handlers, '%s') then love.handlers['%s'] = __NULL__ end
				if not rawget(love.handlers, '%s') then love.handlers['%s'] = __NULL__ end
			--)luastring"--", moduleName, moduleName, handlClr, handlCallback, handlCallback, handlStaticCallback, handlStaticCallback);
            luaR_runCode(L, formatBuffer, "mobsvc.w_signIn");
            memset(formatBuffer, 0, sizeof formatBuffer);

            // call await function in new LÖVE thread
            sprintf(formatBuffer, R"luastring"--(
				require('love.event') require('love.%s') local e, this = love.event, love['%s']
				local success, unlocked = this.incrementAchievementProgressAwait("%s", "%s", %d, %d)
				e.push("%s", success, unlocked) e.push("%s", "%s", "%s", success, unlocked) e.push("%s", { "%s", "%s" })
			--)luastring"--", moduleName, moduleName, androidAchievementId, iosAchievementId, steps, maxSteps, handlCallback, handlStaticCallback, androidAchievementId, iosAchievementId, handlClr, handlCallback, handlStaticCallback);
            luaR_startThread(L, formatBuffer);
            memset(formatBuffer, 0, sizeof formatBuffer);

            for(int i = 0; i < ao; i++) lua_pushnil(L); // reset stack
            return 0;
        }

        int w_incrementAchievementProgressAwait(lua_State *L)
        {
            int n_args = lua_gettop(L);
            long steps = luaL_checklong(L, 3);
            long maxSteps = luaL_checklong(L, 4);
            std::vector<bool> ret;
#if defined(LOVE_ANDROID)
            const char *androidAchievementId = luaL_checkstring(L, 1);
            ret = instance()->incrementAchievementProgressAwait(androidAchievementId, steps, maxSteps);
#elif defined(LOVE_IOS)
            const char *iosAchievementId = luaL_checkstring(L, 2);
            ret = instance()->incrementAchievementProgressAwait(iosAchievementId, steps, maxSteps);
#endif
            lua_pushboolean(L, ret[0]);
            lua_pushboolean(L, ret[1]);
            return 2;
        }

		static const luaL_Reg functions[] =
            {
                { "test", w_test },
                { "signIn", w_signIn }, { "signInAwait", w_signInAwait },
                { "isSignedIn", w_isSignedIn },
                { "showAchievements", w_showAchievements },
                { "incrementAchievementProgress", w_incrementAchievementProgress }, { "incrementAchievementProgressAwait", w_incrementAchievementProgressAwait },
                { 0, 0 }
            };

		extern "C" int luaopen_love_mobsvc(lua_State *L)
		{
			MobSvc *instance = instance();
			if (instance == nullptr) instance = new love::mobsvc::sdl::MobSvc(); else instance->retain();

			WrappedModule w;
			w.module = instance;
			w.name = moduleName;
			w.type = &Module::type;
			w.functions = functions;
			w.types = nullptr;

			int ret = luax_register_module(L, w);

			luaR_runCode(L, R"luastring"--(
				if not love.threaderror then love.threaderror = function(t,m) love.errhand(m ..' (' .. tostring(t) .. ')') end end
				local this = love.mobsvc
				this.__NULL__ = function() end
				this.__clearCallbacks = function(c) for _,n in ipairs(c) do love.handlers[n] = nil end end
				this.newAchievement = function(a,i,ms) return { _androidId = a, _iosId = i, _maxSteps = ms,
				  incrementProgress = function(self, steps) this.incrementAchievementProgress(self._androidId, self._iosId, steps, self._maxSteps) end,
				} end
			--)luastring"--", moduleName);

			return ret;
		}

	} // mobsvc
} // love
