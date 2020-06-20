#define SDL_MAIN_HANDLED

#ifndef PLAYBACK_CPP
#define PLAYBACK_CPP
#include "common.h"
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <SDL2/SDL.h>

#include <iostream> // Debug

struct coordinate {
    float x;
    float y;
};

struct screenDimensions {
    float width;
    float height;
};

int linesToSkip = 3;           // Amount of lines to not read from data (meta data)
int linesRead;
char delimiter = ',';
std::string currentFieldContent;
std::string line;
std::ifstream mapfile(dataFilePath);
std::vector<coordinate> coordinates;
const int WINDOW_WIDTH = 640;  // Width of SDL2 window
const int WINDOW_HEIGHT = 480; // Height of SDL2 window
const int POLLING_RATE = 10;   // Time between mouse position captures in milliseconds
const int CURSOR_SIZE = 10;
float SCALE_FACTOR_WIDTH;      // Equal to screen width divided by window width
float SCALE_FACTOR_HEIGHT;     // Equal to screen height divided by window height
screenDimensions screen;       // Stores screen dimensions

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("Mouse Track Playback", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

int main() {
    if(mapfile.is_open()) {
        while(getline(mapfile, line)) {
            currentFieldContent = "";
            if(linesRead > linesToSkip) {
                coordinate coord;
                for(int i = 0; i < line.length(); i++) {
                    if(line[i] == delimiter) {
                        coord.x = std::stoi(currentFieldContent);
                        currentFieldContent = "";
                    } else {
                        currentFieldContent.push_back(line[i]);
                    }
                }
                coord.y = std::stoi(currentFieldContent);
                coordinates.push_back(coord);
            } else if(linesRead < linesToSkip - 1) {
                bool startReadingMetaData = false;
                for(int i = 0; i < line.length(); i++) {
                    if(line[i] == delimiter) {
                        startReadingMetaData = true;
                    } else if(startReadingMetaData) {
                        currentFieldContent.push_back(line[i]);
                    }
                }
                if(linesRead == 0) {
                    screen.width = std::stoi(currentFieldContent);
                } else if(linesRead == 1) {
                    screen.height = std::stoi(currentFieldContent);
                }
            }
            linesRead++;
        }
    }

    init();

    bool quit = false;
    SDL_Event event;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    long long elapsedTime;
    long long previousTime = -1;

    int currentIndex = -1; // Index of coordinate to read

    SCALE_FACTOR_WIDTH = screen.width / WINDOW_WIDTH;
    SCALE_FACTOR_HEIGHT = screen.height / WINDOW_HEIGHT;

    while(!quit) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);

        if(currentIndex + 1 <= coordinates.size()) {
            if(!(elapsedTime % POLLING_RATE) && elapsedTime != previousTime) {
                previousTime = elapsedTime; // To catch if the next iteration happens during the same elapsed time
                SDL_Rect cursor = {
                    (coordinates[currentIndex].x / SCALE_FACTOR_WIDTH) - (CURSOR_SIZE / 2),
                    (coordinates[currentIndex].y / SCALE_FACTOR_HEIGHT) - (CURSOR_SIZE / 2),
                    CURSOR_SIZE,
                    CURSOR_SIZE
                };
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderFillRect(gRenderer, &cursor);
                SDL_RenderDrawRect(gRenderer, &cursor);
                currentIndex++;
            }
        } else {
            quit = true;
        }

        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}


#endif