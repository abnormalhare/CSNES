#include <SDL2/SDL.h>
#include "nes.h"
#include "opfuncs.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Surface* surface = NULL;

#define VIDEO_WIDTH 256
#define VIDEO_HEIGHT 240

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

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, tWidth, tHeight);
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

uint8_t* convertToRGBA(Dot* ppuScreen) {
    Dot ppuScanline[256];
    char scanline[256];
    uint8_t* screen = malloc(sizeof(uint8_t) * VIDEO_HEIGHT * VIDEO_WIDTH);

    for (int i = 0; i < 240; i++) {
        memccpy(ppuScanline, &ppuScreen[i * 341], 0, 256);
        //...
    }

    return screen;
}

void renderCHRROM(void) {
    const int videoPitch = sizeof(uint32_t) * VIDEO_WIDTH;
    uint8_t isOne;
    int i, j, k;
    uint32_t screen[VIDEO_HEIGHT * VIDEO_WIDTH] = {0};
    uint32_t colorIndexRow[8][8] = {0};
    uint32_t colorRow[8][8] = {0};
    uint32_t colors[4] = {
        0x000000,
        0x888888,
        0xCCCCCC,
        0xFFFFFF
    };
    uint8_t* chr = &chrROM[0];
    for (i = 0; i < 0x100; i++) { // number of 0x10 rows in CHRROM
        for (j = 0; j < 8; j++) { // number of bytes in rows / 2
            for (k = 7; k >= 0; k--) { // number of bits in bytes
                isOne = *chr >> k;
                if (isOne % 2 == 1) {
                    colorIndexRow[j][7 - k] = 1;
                } else {
                    colorIndexRow[j][7 - k] = 0;
                }
            }
            chr++;
        }
        for (j = 0; j < 8; j++) { // number of bytes in rows / 2
            for (k = 7; k >= 0; k--) { // number of bits in bytes
                isOne = *chr >> k;
                if (isOne % 2 == 1) {
                    colorIndexRow[j][7 - k] += 2;
                }
            }
            chr++;
        }
        for (j = 0; j < 64; j++) {
            colorRow[j/8][j%8] = colors[colorIndexRow[j/8][j%8]];
        }

        for (j = 0; j < 8; j++) { // y coordinate
            for (k = 0; k < 8; k++) { // x coordinate
                int y = (j + (i / 0x20 * 8));
                int x = k + (i * 8);
                screen[y * VIDEO_WIDTH + x] = colorRow[j][k];
                // printf("%06X ", screen[y * VIDEO_WIDTH + x]);
            }
            // printf("\n");
        }
    }

    while (1) {
        updateSDL(screen, videoPitch);
    }
}

int main(int argc, char* argv[]) {
    NES* nes;
    uint32_t delay;
    int64_t lastTime;
    FILE* file;
    size_t filesize;
    uint8_t* screen;
    const int videoPitch = sizeof(uint8_t) * VIDEO_WIDTH;

    if (argc != 3) {
        printf("Format: %s <ROM> <scale>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int scale = atoi(argv[2]);

    initSDL(filename, VIDEO_WIDTH * scale, VIDEO_HEIGHT * scale, VIDEO_WIDTH, VIDEO_HEIGHT);

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: Unable to open '%s'.", filename);
        exit(EXIT_FAILURE);
    }

    dbgOutput = fopen("output.txt", "w");
    if (dbgOutput == NULL) {
        printf("ERROR: Unable to open debug output");
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
        if (nes->cycleCount < 29780) {
            nes->cycleFunc(nes);
        } else {
            while (timeInNanoseconds() - lastTime < (1E9 / 60));
            lastTime = timeInNanoseconds();
            screen = convertToRGBA(nes->ppu.screen);
            updateSDL(screen, videoPitch);
            nes->cycleCount = 0;
            nes->ppu.isEvenFrame = ~nes->ppu.isEvenFrame;
        }
    }

    return 0;
}