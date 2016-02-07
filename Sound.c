//
//  Sound.c
//  Dodgeball
//
//  Created by Jesse Pritchard on 3/22/15.
//  Copyright (c) 2015 Jesse Pritchard. All rights reserved.
//

#include "Sound.h"
#include "stdlib.h"

static void AUD_Callback(void* data, Uint8* stream, int len) {
    AUD_Sound* sound = data;
    if (sound->curlen <= 0) {
        return;
    }
    int real_len = (sound->curlen > len ? len : sound->curlen);
    
    memset(stream, 0, len);
    SDL_MixAudioFormat(stream, sound->cur, sound->spec.format, real_len, sound->volume);
    if (real_len == len) {
        sound->cur += real_len;
        sound->curlen -= real_len;
    } 
    else {
        stream += real_len;
        len -= real_len;
        if (sound->loop) {
            sound->cur = sound->buf;
            sound->curlen = sound->buflen;
            
            SDL_MixAudioFormat(stream, sound->cur, sound->spec.format, len, sound->volume);
            
            sound->cur += len;
            sound->curlen -= len;
        }
        else sound->curlen = 0;
    }
}

AUD_Sound* AUD_LoadWAV(const char* wav, int loop) {
    AUD_Sound* product = malloc(sizeof(AUD_Sound));
    if (!product) {
        return NULL;
    }
    
    if (!SDL_LoadWAV(wav, &product->spec, &product->buf, &product->buflen)) {
        free(product);
        return NULL;
    }
    
    product->curlen = product->buflen;
    product->cur = product->buf;
    
    product->spec.callback = AUD_Callback;
    product->spec.userdata = product;
    product->loop = loop;
    
    product->volume = SDL_MIX_MAXVOLUME;
    
    product->dev = SDL_OpenAudioDevice(NULL, 0, &product->spec, NULL, 0);
    if (!product->dev) {
        SDL_FreeWAV(product->buf);
        free(product);
        return NULL;
    }
    return product;
}

void AUD_FreeWav(AUD_Sound* sound) {
    if (sound) {
        if (sound->buf) {
            SDL_FreeWAV(sound->buf);
        }
        SDL_CloseAudioDevice(sound->dev);
        free(sound);
    }
}

void AUD_SetVolume(AUD_Sound* sound, int volume) {
    if (volume > 128) {
        volume = 128;
    }
    else if (volume < 0) {
        volume = 0;
    }
    sound->volume = volume;
}

void AUD_Rewind(AUD_Sound* sound) {
    sound->cur = sound->buf;
    sound->curlen = sound->buflen;
}

void AUD_Play(AUD_Sound* sound) {
    SDL_PauseAudioDevice(sound->dev, 0);
}

void AUD_Pause(AUD_Sound* sound) {
    SDL_PauseAudioDevice(sound->dev, 1);
}