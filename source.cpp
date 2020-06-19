#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include "wtypes.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int CURSOR_SIZE = 10;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
POINT screen, window;
int horizontal, vertical;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("Mouse Track", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

void GetDesktopResolution(int& horizontal, int& vertical) {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

int main(int argc, char* args[]) {
    init();

    bool quit = false;
    SDL_Event event;

    GetDesktopResolution(horizontal, vertical);

    while(!quit) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        if(GetCursorPos(&screen)) {
            screen.x = std::min((int)screen.x, horizontal - 1);
            screen.x = std::max((int)screen.x, 0);
            screen.y = std::min((int)screen.y, vertical - 1);
            screen.y = std::max((int)screen.y, 0);
            SDL_Rect cursor = {
                (screen.x / (horizontal / WINDOW_WIDTH)) - (CURSOR_SIZE / 2),
                (screen.y / (vertical / WINDOW_HEIGHT)) - (CURSOR_SIZE / 2),
                CURSOR_SIZE,
                CURSOR_SIZE
            };
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(gRenderer, &cursor);
            SDL_RenderDrawRect(gRenderer, &cursor);
        }

        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}

//g++ -std=c+=14 source.cpp -o c && c.exe