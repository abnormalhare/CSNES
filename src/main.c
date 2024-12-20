#include <SDL2/SDL.h>
#include "nes.h"
#include "opfuncs.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

#define VIDEO_WIDTH 256
#define VIDEO_HEIGHT 224

void initSDL(char const* title, int wWidth, int wHeight, int tWidth, int tHeight) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: SDL failed to initialize: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wWidth, wHeight, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("ERROR: Window failed to create: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("ERROR: Renderer failed to create: %s\n", SDL_GetError());
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, tWidth, tHeight);
    if (texture == NULL) {
        printf("ERROR: Texture failed to create: %s\n", SDL_GetError());
        return;
    }
}

void exitSDL(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void updateSDL(void const* buffer, int pitch) {
    SDL_UpdateTexture(texture, NULL, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    NES* nes;
    uint32_t delay;
    int64_t lastTime;
    FILE* file;
    size_t filesize;

    if (argc != 3) {
        printf("Format: %s <ROM> <scale>\n", argv[0]);
        return 1;
    }

    char const* filename = argv[1];
    int scale = atoi(argv[2]);

    initSDL(filename, VIDEO_WIDTH * scale, VIDEO_HEIGHT * scale, VIDEO_WIDTH, VIDEO_HEIGHT);

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: Unable to open '%s'.", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    nes = newNES();
    NESLoadROM(nes, file, filename, filesize);

    // setupPC(nes);

    lastTime = timeInNanoseconds();
    while (!nes->jam) {
        nes->cycleFunc(nes);
        if (nes->cycleCount >= 10/*29781*/) {
            // while (timeInNanoseconds() - lastTime < 1E7);
            // lastTime = timeInNanoseconds();
            // nes->cycleCount = 0;
        }
    }

    return 0;
}