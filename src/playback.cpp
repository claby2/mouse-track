#define SDL_MAIN_HANDLED

#ifndef PLAYBACK_CPP
#define PLAYBACK_CPP
#include "common.h"
#include "heatmap.h"
#include "cli.h"
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <SDL2/SDL.h>

struct coordinate {
    float x;
    float y;
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
float SCALE_FACTOR_WIDTH;      // Equal to screen width divided by window width
float SCALE_FACTOR_HEIGHT;     // Equal to screen height divided by window height
int SCREEN_WIDTH;
int SCREEN_HEIGHT;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

/*
Read and input mouse position data from data.csv
*/
void getData() {
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
                InputToHeatmap(coord.x, coord.y); // Input from csv into heatmap
            } else if(linesRead < linesToSkip - 1) { // Read meta data representing screen width and height
                bool startReadingMetaData = false;
                for(int i = 0; i < line.length(); i++) {
                    if(line[i] == delimiter) {
                        startReadingMetaData = true;
                    } else if(startReadingMetaData) {
                        currentFieldContent.push_back(line[i]);
                    }
                }
                if(linesRead == 0) {
                    SCREEN_WIDTH = std::stoi(currentFieldContent);  // Set screen width
                } else if(linesRead == 1) {
                    SCREEN_HEIGHT = std::stoi(currentFieldContent); // Set screen height
                }
            }
            linesRead++;
        }
    }
}

int main() {
    getData();
    init("Mouse Track PLAYBACK");

    bool quit = false;
    SDL_Event event;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    long long elapsedTime;
    long long previousTime = -1;

    int currentIndex = -1; // Index of coordinate to read

    SCALE_FACTOR_WIDTH = SCREEN_WIDTH / WINDOW_WIDTH;
    SCALE_FACTOR_HEIGHT = SCREEN_HEIGHT / WINDOW_HEIGHT;

    long long playbackDuration = POLLING_RATE * coordinates.size();

    CLIPrintPlaybackStart();

    while(!quit) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        CLIPrintPlaybackProgress((float)currentIndex / (float)coordinates.size());

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);

        if(currentIndex + 1 <= coordinates.size()) {
            if(!(elapsedTime % POLLING_RATE) && elapsedTime != previousTime) {
                previousTime = elapsedTime; // To catch if the next iteration happens during the same elapsed time
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                if(currentIndex > 0) {
                    SDL_RenderDrawLine(
                        gRenderer, 
                        (coordinates[currentIndex - 1].x / SCALE_FACTOR_WIDTH),
                        (coordinates[currentIndex - 1].y / SCALE_FACTOR_HEIGHT),
                        (coordinates[currentIndex].x / SCALE_FACTOR_WIDTH),
                        (coordinates[currentIndex].y / SCALE_FACTOR_HEIGHT)
                    );
                }
                currentIndex++;
            }
        } else {
            quit = true;
        }

        SDL_RenderPresent(gRenderer);
    }

    DrawHeatmap();

    close();
    return 0;
}

#endif