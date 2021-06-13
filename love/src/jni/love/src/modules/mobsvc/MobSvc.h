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

#ifndef LOVE_MOBSVC_H
#define LOVE_MOBSVC_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <tuple>
#include <vector>
#include <map>

// LOVE
#include "common/config.h"
#include "common/Module.h"
#include "common/StringMap.h"

// stdlib
#include <string.h>
#include <stdio.h>

// Android
#if defined(LOVE_ANDROID)
#include <jni.h>
#endif

// SDL
#include <SDL_syswm.h>
#include "window/Window.h"

// RICH
#include "common/richutil.h"

namespace love
{
	namespace mobsvc
	{
		class MobSvc : public Module
		{

		public:

			virtual ModuleType getModuleType() const { return M_MOBSVC; }

			MobSvc();
			virtual ~MobSvc() {}

			void test(void) const;

			std::string signInAwait(void);

			bool isSignedIn(void);

            void showAchievements(void);

			std::vector<bool> incrementAchievementProgressAwait(const char *achievementId, int steps, int maxSteps);

		private:

#if defined(LOVE_IOS)
			UIViewController * getRootViewController();
#endif

		}; // MobSvc

	} // mobsvc
} // love

#endif // LOVE_MOBSVC_H
