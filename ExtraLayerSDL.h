/*
    ExtraLayerSDL.h
 
    This file is created by Jesse Pritchard
 
    ALL CHANGES MADE TO SDL 2 OCCUR IN THIS FILE.
    THE ORIGINAL SOURCE AND FUNCTIONALITY OF SDL 2
    REMAINS INTACT IF THIS FILE IS DELETED.
    I TAKE NO CREDIT FOR ANY PART OF THE ORIGINAL 
    SDL 2 DOCUMENTATION OR SOURCE.
 
    SDL 2's original source and documentation can be found at http://wiki.libsdl.org
 
    This is a convoluted little layer that makes a general GeneralLog function
    to work with Mac OS X, as well as an Extra_SDL_Init function to change
    the working directory to the resources folder. These functions can be
    avoided by #defining NOLOGCHANGE and NOINITCHANGE, respectively. In 
    this case, GeneralLog is defined as SDL_Log, and Extra_SDL_Init
    is defined as SDL_Init.
 
    On all but Mac OS, GeneralLog will be defined to be printf if
    NOLOGCHANGE is not defined. This means stdio.h is included. On Mac OS,
    logs will appear in the Console.app. SDL_LOG_CATEGORY_APPLICATION is 
    redefined to the system category. 
 
    TREAD LIGHTLY
*/

#ifndef ExtraLayerSDL_h
#   define ExtraLayerSDL_h

#   ifdef __APPLE__
#       ifdef NOLOGCHANGE
#           define GeneralLog SDL_Log
#       else
#           define SDL_LOG_CATEGORY_APPLICATION SDL_LOG_CATEGORY_SYSTEM
#           define GeneralLog SDL_Log
#       endif //NOLOGCHANGE

#       ifndef NOINITCHANGE
#           include "CoreFoundation/CoreFoundation.h"
            static void SetUpWorkingDirectory(){
                char path[PATH_MAX];
                CFURLRef res = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
                CFURLGetFileSystemRepresentation(res, TRUE, (UInt8 *)path, PATH_MAX);
                CFRelease(res);
                chdir(path);
            }

#           include "SDL.h"
            int Extra_SDL_Init(Uint32 flags){
                int val = SDL_Init(flags);
                SetUpWorkingDirectory();
                return val;
            }

#       else
#           include "SDL.h"
#           define Extra_SDL_Init SDL_Init
#       endif //NOINITCHANGE

#   else //__APPLE__

#       ifdef NOLOGCHANGE
#           define GeneralLog SDL_Log
#       else
#           include "stdio.h"
#           define GeneralLog printf
#       endif //NOLOGCHANGE

#       include "SDL.h"
#       define Extra_SDL_Init SDL_Init

#   endif //__APPLE__

#endif
