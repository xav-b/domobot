#include "XnCommunication.h"

using namespace TUIO;
using boost::asio::ip::tcp;

tcp::iostream socketServer(SERVER_IP, SERVER_PORT);

XnCommunication::XnCommunication() {
    clientIp = "127.0.0.1";
    clientPort = 3333;
}

XnCommunication::XnCommunication(string ip, int port) : clientIp(ip), clientPort(port) {}

void XnCommunication::tuioInit(bool verbose) {
    tuio = new TuioServer(clientIp.c_str(), clientPort, false);
    tuio->setVerbose(verbose);
    dim[0] = 0;
    dim[1] = 640;
    dim[2] = 0;
    dim[3] = 480;
}

void XnCommunication::tuioBlobUpdate(float leader[]) {
    time = TuioTime::getSessionTime();
    tuio->initFrame(time);
    float cursorX = (leader[0] - dim[0]) / (dim[1] - dim[0]);
    float cursorY = (leader[1] - dim[2]) / (dim[3] - dim[2]);
    float cursorZ = leader[2]/10;
    TuioObject* cursor = tuio->getTuioObject(0);
    if ( cursor == NULL || cursor->getTuioTime() == time )
        tuio->addTuioObject(0, cursorX, cursorY, cursorZ);
    else
        tuio->updateTuioObject(cursor, cursorX, cursorY, cursorZ);
}

void XnCommunication::tuioBlobUpdate(XnVHandTracker* Blob) {
    for ( int i = 0; i < Blob->Fingers.size(); i++ ) {
        float cursorX = (Blob->Fingers[i].coordinates.X - dim[0]) / (dim[1] - dim[0]);
        float cursorY = (Blob->Fingers[i].coordinates.Y - dim[2]) / (dim[3] - dim[2]);
        float cursorZ = Blob->Fingers[i].coordinates.Z;
        TuioObject* cursor = tuio->getTuioObject(Blob->Fingers[i].id);
        if ( cursor == NULL || cursor->getTuioTime() == time )
            tuio->addTuioObject(Blob->Fingers[i].id, cursorX, cursorY, cursorZ);
        else
            tuio->updateTuioObject(cursor, cursorX, cursorY, cursorZ);
    }
}

void XnCommunication::tuioCommit() {
    tuio->stopUntouchedMovingObjects();
    tuio->removeUntouchedStoppedObjects();
    tuio->commitFrame();
}

int XnCommunication::socketConnection(std::string jsonMsg) {
    std::ostringstream ss;
    ss << jsonMsg;
    socketServer << ss.str();
    return 0;
}
