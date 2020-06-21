#ifndef CLI_H
#define CLI_H
#include "common.h"
#include <stdio.h>

extern const int TRACKING_POLLING_RATE;
extern long long previousTime;
extern int trackingIndex;
extern const char *TRACKING_ANIMATION_FRAMES[];

/*
Signifies where data will end up
*/
void CLIPrintStart(bool save);

/*
Output to CLI while the mouse is tracking
*/
void CLIPrintTracking(long long elapsedTime);

/*
Signify start of playback
*/
void CLIPrintPlaybackStart();

/*
Returns progress bar string based on progress
*/
const char * getProgressBar(float progress);

/*
Output to CLI the progress of the playback
*/
void CLIPrintPlaybackProgress(float progress);

#endif