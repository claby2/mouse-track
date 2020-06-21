#ifndef COMMON_H
#define COMMON_H
#include <SDL2/SDL.h>
#include <stdbool.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int CURSOR_SIZE;
extern const int POLLING_RATE;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern bool save;
extern char dataFilePath[];
extern char heatmapFilePath[];

/*
Initialize SDL2
*/
void init(char windowName[]);

/*
Close SDL2
*/
void close();

#endif