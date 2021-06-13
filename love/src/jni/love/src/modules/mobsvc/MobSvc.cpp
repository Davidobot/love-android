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

#include <iostream>
#include <stdio.h>
#include <string>
#include <tuple>
#include <vector>
#include <map>

// LOVE
#include "common/config.h"
#include "MobSvc.h"
#include "config_MobSvc.h"

#if defined(LOVE_MACOSX)
#include <CoreServices/CoreServices.h>
#elif defined(LOVE_IOS)
#include "common/ios.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioServices.h>
#elif defined(LOVE_LINUX) || defined(LOVE_ANDROID)
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#elif defined(LOVE_WINDOWS)
#include "common/utf8.h"
#include <shlobj.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#endif
#if defined(LOVE_ANDROID)
#include "common/android.h"
#elif defined(LOVE_LINUX)
#include <spawn.h>
#endif


namespace love
{
	namespace mobsvc
	{
		MobSvc::MobSvc()
		{
#if defined(MOBSVC_ANDROID_PLAY_GAMES_PERMISSION_GDRIVE)
            love::android::mobSvcInit(true);
#else
            love::android::mobSvcInit(false);
#endif
		}

		void MobSvc::test() const
		{
			printf("MOBSVC_TEST\n");
		}

        std::string MobSvc::signInAwait()
		{
			return love::android::mobSvcSignInAwait();
		}

		bool MobSvc::isSignedIn()
		{
			return love::android::mobSvcIsSignedIn();
		}

        void MobSvc::showAchievements(void)
        {
            love::android::mobSvcShowAchievements();
        }

        std::vector<bool> MobSvc::incrementAchievementProgressAwait(const char *achievementId, int steps, int maxSteps)
        {
            return love::android::mobSvcIncrementAchievementProgressAwait(achievementId, steps, maxSteps);
        }
	}
}
