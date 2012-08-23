#ifndef DEF_COMM_H
#define DEF_COMM_H
 
//#include "HandTracker.cpp"
//#include <iostream>
#include <sstream>
//#include <string>
#include <TuioServer.h>
#include <list>
#include <boost/asio.hpp>
#include "HandTracker.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "10002"

/**
 * 
 */
class XnCommunication
{
public:
	XnCommunication();
	XnCommunication(std::string ip, int port);
    void tuioInit(bool verbose);
    void tuioBlobUpdate(float leader[]);
    void tuioBlobUpdate(XnVHandTracker* Blob);
    void tuioCommit();
    int socketConnection(std::string jsonMsg);

protected:
    TUIO::TuioServer* tuio;
    TUIO::TuioTime time;
    std::string clientIp;
    int clientPort;
    int dim[4];
};

#endif
