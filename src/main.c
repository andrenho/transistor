#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#include <SDL3/SDL.h>

#include "pastel2d.h"
#include "transistor-sandbox.h"

int main(void)
{
    const int linked = SDL_GetVersion();
    SDL_Log("SDL version %d.%d.%d", SDL_VERSIONNUM_MAJOR(linked), SDL_VERSIONNUM_MINOR(linked), SDL_VERSIONNUM_MICRO(linked));
    SDL_Log("pastel2d version %s", ps_version(NULL, NULL, NULL));
    SDL_Log("transistor-sandbox version %s", ts_transistor_version(NULL, NULL, NULL));

    printf("Hello, World!\n");
    return 0;
}
