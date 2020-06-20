#ifndef COMMON_H
#define COMMON_H
#include <SDL2/SDL.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int CURSOR_SIZE;
extern const int POLLING_RATE;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern bool save;
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

#endif