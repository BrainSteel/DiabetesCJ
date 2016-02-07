//
//  Sound.h
//  Dodgeball
//
//  Created by Jesse Pritchard on 3/22/15.
//  Copyright (c) 2015 Jesse Pritchard. All rights reserved.
//

#ifndef __Dodgeball__Sound__
#define __Dodgeball__Sound__

#include "SDL.h"

typedef struct AUD_sSound{
    SDL_AudioSpec spec;
    int loop, volume;
    SDL_AudioDeviceID dev;
    Uint8* buf, *cur;
    Uint32 buflen, curlen;
} AUD_Sound;

AUD_Sound* AUD_LoadWAV(const char* wav, int loop);
void AUD_FreeWav(AUD_Sound* sound);

void AUD_SetVolume(AUD_Sound* sound, int volume);
void AUD_Rewind(AUD_Sound* sound);

void AUD_Play(AUD_Sound* sound);
void AUD_Pause(AUD_Sound* sound);

#endif /* defined(__Dodgeball__Sound__) */
