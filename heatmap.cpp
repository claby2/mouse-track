#ifndef HEATMAP_CPP
#define HEATMAP_CPP
#include "./constants.cpp"

const int HEATMAP_WIDTH = 100;
const int HEATMAP_HEIGHT = 100;

long long heatmap[HEATMAP_HEIGHT][HEATMAP_WIDTH];

void InputToHeatmap(double screenX, double screenY) {
    heatmap[(int)screenY / (SCREEN_HEIGHT / HEATMAP_HEIGHT)][(int)screenX / (SCREEN_WIDTH / HEATMAP_WIDTH)]++;
}

#endif