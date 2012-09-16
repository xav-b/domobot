#include "XnCommunication.h"

using boost::asio::ip::tcp;

tcp::iostream socketServer(SERVER_IP, SERVER_PORT);

XnCommunication::XnCommunication() {
    clientIp = "127.0.0.1";
    clientPort = 10002;
    for (int i = 1; i < 6; i++) {
        trackedHand.push_back(false);
    }
    handBuffer.push(trackedHand);
    dim[0] = 0;
    dim[1] = 640;
    dim[2] = 0;
    dim[3] = 480;
}

XnCommunication::XnCommunication(string ip, int port) : clientIp(ip), clientPort(port) {
    for (int i = 1; i < 6; i++) {
        trackedHand.push_back(false);
    }
    handBuffer.push(trackedHand);
    dim[0] = 0;
    dim[1] = 640;
    dim[2] = 0;
    dim[3] = 480;
}

void XnCommunication::composeJsonMsg(float leader[]) {
    std::ostringstream ss;
    //ss << "{\"id\":" << 0 << ", \"state\":" << 0 << ", \"X\":" << m_pointTracked.X/dim[0] << ", \"Y\":" << m_pointTracked.Y/dim[2] << ", \"Z\":" << m_pointTracked.Z << "}";
    ss << "{\"id\":" << 0 << ", \"state\":" << 0 << ", \"X\":" << leader[0]/dim[1] << ", \"Y\":" << leader[1]/dim[3] << ", \"Z\":" << leader[2] << "}";
    socketServer << ss.str();
    std::cout << ss.str() << std::endl;
}

void XnCommunication::composeJsonMsg(XnVHandTracker* Blob) {
    trackedHand.clear();
    for (int i = 1; i < 6; i++) {
        trackedHand.push_back(false);
    }
    trackedHand[0] = true;
    for (int i = 0; i < Blob->Fingers.size(); i++ ) {
        std::ostringstream ss;
        trackedHand[Blob->Fingers[i].id] = true;
        if ( handBuffer.front()[Blob->Fingers[i].id] ) {
            std::cout << "Updating cursor " << Blob->Fingers[i].id << std::endl;
            Blob->Fingers[i].state = 0;
        }
        else {
            Blob->Fingers[i].state = 1;
            std::cout << "Creating cursor " << Blob->Fingers[i].id << std::endl;
        }
        ss << "{\"id\":" << Blob->Fingers[i].id << ", \"state\":" << Blob->Fingers[i].state << ", \"X\":" << Blob->Fingers[i].coordinates.X << ", \"Y\":" << Blob->Fingers[i].coordinates.Y << ", \"Z\":" << Blob->Fingers[i].coordinates.Z << "}";
        socketServer << ss.str();
        std::cout << ss.str() << std::endl;
    }
    for (int i=0; i < 6; i++) {
        std::ostringstream ss;
        if ( !trackedHand[i] && handBuffer.front()[i] ) {
            std::cout << "Deleting cursor " << i << std::endl;
            ss << "{\"id\":" << i << ", \"state\":" << -1 << std::endl; 
            socketServer << ss.str();
            std::cout << ss.str() << std::endl;
        }
    }
    handBuffer.push(trackedHand);
    handBuffer.pop();
}

void XnCommunication::shutdown() {
    std::ostringstream ss;
    ss << ";";
    socketServer << ss.str();
}

//int XnCommunication::socketConnection(std::string jsonMsg) {
    //std::ostringstream ss;
    //ss << jsonMsg;
    //socketServer << ss.str();
    //return 0;
//}
