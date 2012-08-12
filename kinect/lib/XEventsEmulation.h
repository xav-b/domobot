#ifndef DEF_XEVENTS
#define DEF_XEVENTS

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
// Needed for processing interface
#include <vector>
#include <cv.h>
#include <boost/asio.hpp>

//TODO Put defines in config file
#define AVERAGE_BUFFER 8
#define X_OFFSET 150
#define Y_OFFSET 70
#define SCALE 4
#define SCREEN_WIDTH 1366
#define SCREEN_HIGH 768

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "10002"

//using boost::asio::ip::tcp;

class XEventsEmulation {
    public:
        XEventsEmulation();
        void calibrateScreen();
        int smoothingPoint(int point, char axe);
        int mouseClick(std::string button, std::string type);
        int mouseMove(int x, int y);
        int keyHit(std::string key, std::string type);
        int processingUI(float rh[3], vector<cv::Point> fingerTips);

    private:
        int xCoordinatesBuffer[4];
        int yCoordinatesBuffer[4];
        int screenWidth;
        int screenHigh;
        int kinectPosition[3];
        //tcp::iostream m_server;
};

#endif
