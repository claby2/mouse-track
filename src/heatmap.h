#ifndef HEATMAP_H
#define HEATMAP_H
#include "common.h"
#include <SDL2/SDL.h>
#include <limits.h>

const int HEATMAP_WIDTH = 16;
const int HEATMAP_HEIGHT = 16;
const int HEATMAP_WINDOW_SIZE = 600;                               // Represents witdth and height
const int HEATMAP_CELL_SIZE = HEATMAP_WINDOW_SIZE / HEATMAP_WIDTH; // Size of one grid cell on the heatmap
long long heatmap[HEATMAP_HEIGHT][HEATMAP_WIDTH];                  // Stores frequency coordinate appears when normalized
long long heatmapMinimum = LONG_MAX;                               // Maximum frequency value in heatmap
long long heatmapMaximum = 0;                                      // Minimum frequency value in heatmap
const int COLORMAP_CATEGORIES = 4;                                 // The amount of color categories which belong to the colormap
const int NORMALIZED_MAXIMUM = 100;                                // The maximum value the frequency can take when normalized
const int COLOR_MAXIMUM = 255;                                     // Maximum value for a single color channel
const float COLORMAP_GRADIENT = 10.2;                              // Calculated by the COLOR_MAXIMUM / (NORMALIZED_MAXIMUM / COLORMAP_CATEGORIES)

/* 
Increment frequency in heatmap
*/
void InputToHeatmap(double screenX, double screenY) {
    heatmap[(int)screenY / (SCREEN_HEIGHT / HEATMAP_HEIGHT)][(int)screenX / (SCREEN_WIDTH / HEATMAP_WIDTH)]++;
}

/* 
Set the maximum and minimum value to the highest and lowest frequency respectively
*/
void GetHeatmapBounds(long long *max, long long *min) {
    for(int i = 0; i < HEATMAP_HEIGHT; i++) {
        for(int j = 0; j < HEATMAP_WIDTH; j++) {
            if(heatmap[i][j] > *max) *max = heatmap[i][j];
            if(heatmap[i][j] < *min) *min = heatmap[i][j];
        }
    }
}

/* 
Normalize value into 0 to 100 range based on minimum and maximum frequency of heatmap
*/
int NormalizeValue(long long value) {
    return (int)(NORMALIZED_MAXIMUM * ((float)(value - heatmapMinimum) / (heatmapMaximum - heatmapMinimum)));
}

/*
Save SDL2 heatmap window as BMP image
*/
void SaveAsBMP() {
    SDL_Surface *surface = SDL_CreateRGBSurface(
        0, 
        HEATMAP_WINDOW_SIZE, 
        HEATMAP_WINDOW_SIZE, 
        32, 
        0x00ff0000, 
        0x0000ff00, 
        0x000000ff, 
        0xff000000
    );
    SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    SDL_SaveBMP(surface, heatmapFilePath);
    SDL_FreeSurface(surface);
}

/*
Get RGB value based on normalized value in range 0 to 100 to generate colormap
*/
void GetRGB(int normalizedValue, int *red, int *green, int *blue) {
    if(normalizedValue <= NORMALIZED_MAXIMUM / COLORMAP_CATEGORIES) {
        *red = 0;
        *green = COLORMAP_GRADIENT * normalizedValue;                // Only increasing green
        *blue = 255;
    } else if(normalizedValue <= 2 * (NORMALIZED_MAXIMUM / COLORMAP_CATEGORIES)) {
        *red = 0;
        *green = 255;
        *blue = 255 - (COLORMAP_GRADIENT * (normalizedValue - 25));  // Only decreasing blue
    } else if(normalizedValue <= 3 * (NORMALIZED_MAXIMUM / COLORMAP_CATEGORIES)) {
        *red = COLORMAP_GRADIENT * (normalizedValue - 50);           // Only increasing red
        *green = 255;
        *blue = 0;
    } else if(normalizedValue <= NORMALIZED_MAXIMUM) {
        *red = 255;
        *green = 255 - (COLORMAP_GRADIENT * (normalizedValue - 75)); // Only decreasing green
        *blue = 0;
    }
}

void DrawHeatmap() {
    GetHeatmapBounds(&heatmapMaximum, &heatmapMinimum);

    SDL_SetWindowSize(gWindow, HEATMAP_WINDOW_SIZE, HEATMAP_WINDOW_SIZE);
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); // Clear window
    for(int i = 0; i < HEATMAP_HEIGHT; i++) {
        for(int j = 0; j < HEATMAP_WIDTH; j++) {
            int red, green, blue;
            GetRGB(NormalizeValue(heatmap[i][j]), &red, &green, &blue);
            SDL_SetRenderDrawColor(gRenderer, red, green, blue, 255);
            SDL_Rect cell = {
                HEATMAP_CELL_SIZE * j,
                HEATMAP_CELL_SIZE * i,
                HEATMAP_CELL_SIZE,
                HEATMAP_CELL_SIZE
            };
            SDL_RenderFillRect(gRenderer, &cell);
            SDL_RenderDrawRect(gRenderer, &cell);

        }
    }
    SDL_RenderPresent(gRenderer);

    SaveAsBMP();
}

#endif