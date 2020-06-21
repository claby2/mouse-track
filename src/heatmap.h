#ifndef HEATMAP_H
#define HEATMAP_H
#include "common.h"
#include <SDL2/SDL.h>
#include <limits.h>

extern const int HEATMAP_WIDTH;
extern const int HEATMAP_HEIGHT;
extern const int HEATMAP_WINDOW_SIZE;
extern const int HEATMAP_CELL_SIZE;
extern long long heatmap[];
extern long long heatmapMinimum;
extern long long heatmapMaximum;
extern const int COLORMAP_CATEGORIES;
extern const int NORMALIZED_MAXIMUM;
extern const int COLOR_MAXIMUM;
extern const float COLORMAP_GRADIENT;

/* 
Increment frequency in heatmap
*/
void InputToHeatmap(double screenX, double screenY);

/* 
Set the maximum and minimum value to the highest and lowest frequency respectively
*/
void GetHeatmapBounds(long long *max, long long *min);

/* 
Normalize value into 0 to 100 range based on minimum and maximum frequency of heatmap
*/
int NormalizeValue(long long value);

/*
Save SDL2 heatmap window as BMP image
*/
void SaveAsBMP();

/*
Get RGB value based on normalized value in range 0 to 100 to generate colormap
*/
void GetRGB(int normalizedValue, int *red, int *green, int *blue);

/*
Draw and call function to save heatmap as BMP
*/
void DrawHeatmap();

#endif