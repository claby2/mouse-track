#include "common.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 640;  // Width of SDL2 window
const int WINDOW_HEIGHT = 480; // Height of SDL2 window
const int CURSOR_SIZE = 10;
const int POLLING_RATE = 10;   // Time between mouse position captures in milliseconds
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
bool save = true;              // Flag to decide whether to save to data.csv or not

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

char dataFilePath[] = "../data.csv";
char heatmapFilePath[] = "../heatmap.bmp";

void init(char windowName[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
}