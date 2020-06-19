#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include "wtypes.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int CURSOR_SIZE = 10;
const int POLLING_RATE = 10; // Time between mouse position captures in milliseconds

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
POINT screen;
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

    std::ofstream file;
    file.open("data.csv");
    file << "x position, y position\n";

    bool quit = false;
    SDL_Event event;

    GetDesktopResolution(horizontal, vertical); // Assigns screen width and height

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    long long elapsedTime;
    long long previousTime = -1;

    while(!quit) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        if(GetCursorPos(&screen)) {
            screen.x = std::min((int)screen.x, horizontal - 1); // Set maximum x bounds
            screen.x = std::max((int)screen.x, 0);              // Set minimum x bounds
            screen.y = std::min((int)screen.y, vertical - 1);   // Set maximum y bounds
            screen.y = std::max((int)screen.y, 0);              // Set minimum y bounds
            SDL_Rect cursor = {
                (screen.x / (horizontal / WINDOW_WIDTH)) - (CURSOR_SIZE / 2), // Format x screen position relative to window size
                (screen.y / (vertical / WINDOW_HEIGHT)) - (CURSOR_SIZE / 2),  // Format y screen position relative to window size
                CURSOR_SIZE,
                CURSOR_SIZE
            };
            // Draw and fill the cursor
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(gRenderer, &cursor);
            SDL_RenderDrawRect(gRenderer, &cursor);
            if(!(elapsedTime % POLLING_RATE) && elapsedTime != previousTime) {
                previousTime = elapsedTime;
                file << horizontal - screen.x << ',' << vertical - screen.y << "\n"; // Write x position and y position into csv
            }
        }

        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}