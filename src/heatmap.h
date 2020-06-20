#ifndef HEATMAP_H
#define HEATMAP_H
#include "common.h"
#include <SDL2/SDL.h>
#include <limits.h>

const int HEATMAP_WIDTH = 50;
const int HEATMAP_HEIGHT = 50;
long long heatmap[HEATMAP_HEIGHT][HEATMAP_WIDTH]; // Stores frequency coordinate appears when normalized
long long heatmapMinimum = LONG_MAX;
long long heatmapMaximum = 0;

/* 
Increment frequency in heatmap
*/
void InputToHeatmap(double screenX, double screenY) {
    heatmap[(int)screenY / (SCREEN_HEIGHT / HEATMAP_HEIGHT)][(int)screenX / (SCREEN_WIDTH / HEATMAP_WIDTH)]++;
}

/* 
Set the maximum and minimum value to the highest and lowest frequency respectively
*/
void GetHeatmapBounds(long long& max, long long& min) {
    for(int i = 0; i < HEATMAP_HEIGHT; i++) {
        for(int j = 0; j < HEATMAP_WIDTH; j++) {
            if(heatmap[i][j] > max) max = heatmap[i][j];
            if(heatmap[i][j] < min) min = heatmap[i][j];
        }
    }
}

/* 
Normalize value into 0 to 255 range based on minimum and maximum frequency of heatmap
*/
int NormalizeValue(long long value) {
    return (int)(255 * ((float)(value - heatmapMinimum) / (heatmapMaximum - heatmapMinimum)));
}

void SaveAsBMP() {
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 
        HEATMAP_WIDTH, 
        HEATMAP_HEIGHT, 
        32, 
        0x00ff0000, 
        0x0000ff00, 
        0x000000ff, 
        0xff000000
    );
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, "../heatmap.bmp");
    SDL_FreeSurface(surface);
}

void DrawHeatmap() {
    SDL_Texture* gTexture = SDL_CreateTexture(
        gRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        HEATMAP_WIDTH,
        HEATMAP_HEIGHT
    );

    uint32_t pixels[HEATMAP_HEIGHT][HEATMAP_WIDTH];

    GetHeatmapBounds(heatmapMaximum, heatmapMinimum);

    for(int i = 0; i < HEATMAP_HEIGHT; i++) {
        for(int j = 0; j < HEATMAP_WIDTH; j++) {
            pixels[i][j] = 0xffffffffu - (NormalizeValue(heatmap[i][j]) << 8) - NormalizeValue(heatmap[i][j]);
        }
    }

    SDL_SetWindowSize(gWindow, HEATMAP_WIDTH, HEATMAP_HEIGHT);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); // Clear window
    SDL_UpdateTexture(gTexture, NULL, &pixels[0], HEATMAP_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
    SDL_RenderPresent(gRenderer);

    SaveAsBMP();

    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
}

#endif