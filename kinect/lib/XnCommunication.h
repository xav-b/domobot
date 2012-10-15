#ifndef DEF_COMM_H
#define DEF_COMM_H
 
#include <sstream>
#include <vector>
#include <queue>
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
    void composeJsonMsg(XnVHandTracker* Blob);
    void composeJsonMsg(float leader[]);
    void shutdown();
    //int socketConnection(std::string jsonMsg);

protected:
    std::string clientIp;
    std::vector<bool> trackedHand;
    std::queue< std::vector<bool> > handBuffer;
    int clientPort;
    int dim[6];
};

#endif
