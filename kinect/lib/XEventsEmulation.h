#ifndef DEF_XEVENTS
#define DEF_XEVENTS

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
// Needed for processing interface
#include <vector>
#include <cv.h>

//TODO Put defines in config file
#define AVERAGE_BUFFER 8
#define X_OFFSET 150
#define Y_OFFSET 70
#define SCALE 4
#define SCREEN_WIDTH 1366
#define SCREEN_HIGH 768

class XEventsEmulation {
    public:
        XEventsEmulation();
        void calibrateScreen();
        int smoothingPoint(int point, char axe);
        int mouseClick(std::string button, std::string type);
        int mouseMove(int x, int y);
        int keyHit(std::string key, std::string type);

    private:
        int xCoordinatesBuffer[4];
        int yCoordinatesBuffer[4];
        int screenWidth;
        int screenHigh;
        int kinectPosition[3];
};

#endif
