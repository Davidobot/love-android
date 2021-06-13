/**
 * Created by Martin Braun (modiX) for love2d (copied and modified from AdMob port by bio1712)
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

#ifndef LOVE_MSVC_SDL_MSVC_H
#define LOVE_MSVC_SDL_MSVC_H

// LOVE
#include "../MobSvc.h"
#include "common/EnumMap.h"

// SDL
#include <SDL_power.h>

namespace love
{
	namespace mobsvc
	{
		namespace sdl
		{
			class MobSvc : public love::mobsvc::MobSvc
			{
				
			public:
				
				MobSvc();
				virtual ~MobSvc() {}
				
				// Implements Module.
				const char *getName() const;
				
			private:
				
				
			}; // MobSvc
		} // sdl
	} // mobsvc
} // love

#endif // LOVE_MOBSVC_SDL_MSVC_H
