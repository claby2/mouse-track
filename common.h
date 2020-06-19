#ifndef COMMON_H
#define COMMON_H
#include <SDL2/SDL.h>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int CURSOR_SIZE;
extern const int POLLING_RATE; // Time between mouse position captures in milliseconds
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

#endif