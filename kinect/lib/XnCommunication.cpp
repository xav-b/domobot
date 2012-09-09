#include "XnCommunication.h"

//using namespace TUIO;
using boost::asio::ip::tcp;

tcp::iostream socketServer(SERVER_IP, SERVER_PORT);

XnCommunication::XnCommunication() {
    clientIp = "127.0.0.1";
    clientPort = 3333;
    for (int i = 1; i < 6; i++) {
        trackedHand.push_back(false);
    }
    handBuffer.push(trackedHand);
}

XnCommunication::XnCommunication(string ip, int port) : clientIp(ip), clientPort(port) {
    for (int i = 1; i < 6; i++) {
        trackedHand.push_back(false);
    }
    handBuffer.push(trackedHand);
}

//void XnCommunication::tuioInit(bool verbose) {
    //tuio = new TuioServer(clientIp.c_str(), clientPort, false);
    //tuio->setVerbose(verbose);
    //dim[0] = 0;
    //dim[1] = 640;
    //dim[2] = 0;
    //dim[3] = 480;
//}

//void XnCommunication::tuioBlobUpdate(float leader[]) {
    //trackedHand.clear();
    //for (int i = 1; i < 6; i++) {
        //trackedHand.push_back(false);
    //}
    //trackedHand[0] = true;
    //time = TuioTime::getSessionTime();
    //tuio->initFrame(time);
    //float cursorX = (leader[0] - dim[0]) / (dim[1] - dim[0]);
    //float cursorY = (leader[1] - dim[2]) / (dim[3] - dim[2]);
    //float cursorZ = leader[2]/10;
    //TuioObject* cursor = tuio->getTuioObject(0);
    //if ( cursor == NULL || cursor->getTuioTime() == time )
        //tuio->addTuioObject(0, cursorX, cursorY, cursorZ);
    //else
        //tuio->updateTuioObject(cursor, cursorX, cursorY, cursorZ);
//}

//void XnCommunication::tuioBlobUpdate(XnVHandTracker* Blob) {
    //for (int i = 0; i < Blob->Fingers.size(); i++ ) {
        //trackedHand[Blob->Fingers[i].id] = true;
        //float cursorX = (Blob->Fingers[i].coordinates.X - dim[0]) / (dim[1] - dim[0]);
        //float cursorY = (Blob->Fingers[i].coordinates.Y - dim[2]) / (dim[3] - dim[2]);
        //float cursorZ = Blob->Fingers[i].coordinates.Z;
        //TuioObject* cursor = tuio->getTuioObject(Blob->Fingers[i].id);
        //if ( cursor == NULL || cursor->getTuioTime() == time ) {
            //tuio->addTuioObject(Blob->Fingers[i].id, cursorX, cursorY, cursorZ);
            //std::cout << "Creating cursor " << i+1 << std::endl;
        //}
        //else
            //tuio->updateTuioObject(cursor, cursorX, cursorY, cursorZ);
    //}
    ////handBuffer.push(trackedHand);
//}

//void XnCommunication::tuioCommit() {
    //std::cout << "Commiting frame " << tuio->getFrameID() << " of session " << tuio->getSessionID() << std::endl;
    //std::list<TuioObject*> cursorList = tuio->getTuioObjects();
    //std::list<TuioObject*> inactiveList = tuio->getUntouchedObjects();
    //std::cout << "Active " << cursorList.size() << " cursors / Inactive " << inactiveList.size()  << "\n";
    //for (std::list<TuioObject*>::iterator tuioCursor = cursorList.begin(); tuioCursor!=cursorList.end(); tuioCursor++) {
        //if ( !trackedHand[(*tuioCursor)->getSymbolID()] ) {
            //tuio->removeTuioObject(*tuioCursor);
            //std::cout << "Deleting finger " << (*tuioCursor)->getSymbolID() << std::endl;
        //}
    //}
    //tuio->stopUntouchedMovingObjects();
    //tuio->removeUntouchedStoppedObjects();
    //tuio->commitFrame();
    ////tuio->sendFullMessages();
    ////handBuffer.pop();
//}

//void XnCommunication::updateBuffer() {
//}

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

//int XnCommunication::socketConnection(std::string jsonMsg) {
    //std::ostringstream ss;
    //ss << jsonMsg;
    //socketServer << ss.str();
    //return 0;
//}
