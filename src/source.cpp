#define SDL_MAIN_HANDLED

#include "common.h"
#include "heatmap.h"
#include "cli.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include "wtypes.h"

const int WINDOW_WIDTH = 640;  // Width of SDL2 window
const int WINDOW_HEIGHT = 480; // Height of SDL2 window
const int CURSOR_SIZE = 10;
const int POLLING_RATE = 10;   // Time between mouse position captures in milliseconds
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
float SCALE_FACTOR_WIDTH;      // Equal to screen width divided by window width
float SCALE_FACTOR_HEIGHT;     // Equal to screen height divided by window height
bool save = true;              // Flag to decide whether to save to data.csv or not

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
POINT screen;

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

void GetDesktopResolution(int& SCREEN_WIDTH, int& SCREEN_HEIGHT) {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    SCREEN_WIDTH = desktop.right;
    SCREEN_HEIGHT = desktop.bottom;
}

int main(int argc, char* args[]) {
    if(argc > 1) {
        std::string arg = args[1];
        if(arg == "--save") {
            save = true;
        } else if(arg == "--nosave") {
            save = false;
        }
    }
    init();
    
    bool quit = false;
    SDL_Event event;

    GetDesktopResolution(SCREEN_WIDTH, SCREEN_HEIGHT); // Assigns screen width and height

    std::ofstream file;
    if(save) {
        file.open("../data.csv");
        file << "#w," << SCREEN_WIDTH << "\n";
        file << "#h," << SCREEN_HEIGHT << "\n";
        file << "x position, y position\n";
    }

    SCALE_FACTOR_WIDTH = SCREEN_WIDTH / WINDOW_WIDTH;
    SCALE_FACTOR_HEIGHT = SCREEN_HEIGHT / WINDOW_HEIGHT;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    long long elapsedTime;
    long long previousTime = -1;

    CLIPrintStart(save);

    while(!quit) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        CLIPrintTracking(elapsedTime);

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        if(GetCursorPos(&screen)) {
            screen.x = std::min((int)screen.x, SCREEN_WIDTH - 1);  // Set maximum x bounds
            screen.x = std::max((int)screen.x, 0);                 // Set minimum x bounds
            screen.y = std::min((int)screen.y, SCREEN_HEIGHT - 1); // Set maximum y bounds
            screen.y = std::max((int)screen.y, 0);                 // Set minimum y bounds
            SDL_Rect cursor = {
                (screen.x / SCALE_FACTOR_WIDTH) - (CURSOR_SIZE / 2),   // Format x screen position relative to window size
                (screen.y / SCALE_FACTOR_HEIGHT) - (CURSOR_SIZE / 2),  // Format y screen position relative to window size
                CURSOR_SIZE,
                CURSOR_SIZE
            };
            // Draw and fill the cursor
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(gRenderer, &cursor);
            SDL_RenderDrawRect(gRenderer, &cursor);
            if(!(elapsedTime % POLLING_RATE) && elapsedTime != previousTime && save) {
                previousTime = elapsedTime;
                file << screen.x << ',' << SCREEN_HEIGHT - screen.y << "\n"; // Write x position and y position into csv
            }
            InputToHeatmap(screen.x, screen.y);
        }

        SDL_RenderPresent(gRenderer);
    }

    DrawHeatmap();
    close();
    return 0;
}