#include "cli.h"

const int TRACKING_POLLING_RATE = 100;                            // Time between tracking animation frames (CLI output)
long long previousTime = -1;
int trackingIndex;                                                // Stage of tracking animation frames (CLI output)
const char *TRACKING_ANIMATION_FRAMES[4] = {"\\", "|", "/", "-"}; // Frames for tracking animation (CLI output)

void CLIPrintStart(bool save) {
    printf("Starting Mouse Track capture.\n\n");
    if(save) {
        printf("\t- Data will be written to %s\n", dataFilePath);
    }
    printf("\t- Heatmap will be written to %s\n\n", heatmapFilePath);
}

void CLIPrintTracking(long long elapsedTime) {
    if(!(elapsedTime % TRACKING_POLLING_RATE) && elapsedTime != previousTime) {
        previousTime = elapsedTime; // To catch if the next iteration happens during the same elapsed time
        trackingIndex = trackingIndex < sizeof(TRACKING_ANIMATION_FRAMES)/sizeof(*TRACKING_ANIMATION_FRAMES) - 1 ? trackingIndex + 1 : 0;
        printf("TRACKING %s\r", TRACKING_ANIMATION_FRAMES[trackingIndex]);
        fflush(stdout);
    }
}

void CLIPrintPlaybackStart() {
    printf("Starting Mouse Track playback based on %s\n\n", dataFilePath);
}

const char * getProgressBar(float progress) {
    static char bar[] = "----------";
    for(int i = 0; i < ((int)(progress * 10)) % 10; i++) {
        bar[i] = '#';
    }
    return bar;
}

void CLIPrintPlaybackProgress(float progress) {
    if(progress != 1) {
        printf("<%s> %d%% PLAYING\r", getProgressBar(progress), (int)(progress * 100));
    } else {
        printf("<##########> 100%% PLAYING\r");
    }
    fflush(stdout);
}