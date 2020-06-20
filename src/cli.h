#ifndef CLI_H
#define CLI_H
#include "common.h"
#include <iostream>

const int TRACKING_POLLING_RATE = 100;                                  // Time between tracking animation frames (CLI output)
long long previousTime = -1;
int index;                                                              // Stage of tracking animation frames (CLI output)
const std::string TRACKING_ANIMATION_FRAMES[4] = {"\\", "|", "/", "-"}; // Frames for tracking animation (CLI OUTPUT)

/*
Signifies where data will end up
*/
void CLIPrintStart(bool save) {
    std::cout << "Starting Mouse Track capture.\n\n";
    if(save) {
        std::cout << "  - Data will be written to " << dataFilePath << "\n";
    }
    std::cout << "  - Heatmap will be written to " << heatmapFilePath << "\n\n";
}

/*
Output to CLI while the mouse is tracking
*/
void CLIPrintTracking(long long elapsedTime) {
    if(!(elapsedTime % TRACKING_POLLING_RATE) && elapsedTime != previousTime) {
        previousTime = elapsedTime; // To catch if the next iteration happens during the same elapsed time
        index = index < sizeof(TRACKING_ANIMATION_FRAMES)/sizeof(*TRACKING_ANIMATION_FRAMES) - 1 ? index + 1 : 0;
        std::cout << "TRACKING " << TRACKING_ANIMATION_FRAMES[index] << "\r";
        std::cout.flush();
    }
}

/*
Signify start of playback
*/
void CLIPrintPlaybackStart() {
    std::cout << "Starting Mouse Track playback based on " << dataFilePath << "\n";
}

/*
Output to CLI the progress of the playback
*/
void CLIPrintPlaybackProgress(float progress) {
    if(progress != 1) {
        std::cout << '<' << 
        std::string(((int)(progress * 10)) % 10, '#') << 
        std::string( 10 - (((int)(progress * 10)) % 10), '-') <<
        "> " <<
        (int)(progress * 100) << 
        "% PLAYING" << "\r";
    } else {
        std::cout << "<##########> 100\% PLAYING" << "\r";
    }
    std::cout.flush();
}

#endif