#include "XnCommunication.h"

using boost::asio::ip::tcp;

tcp::iostream socketServer(SERVER_IP, SERVER_PORT);

XnCommunication::XnCommunication() {
    clientIp = "127.0.0.1";
    clientPort = 10002;
    /*
     *for (int i = 1; i < 6; i++) {
     *    trackedHand.push_back(false);
     *}
     *handBuffer.push(trackedHand);
     */
    dim[0] = 0;
    dim[1] = 640;
    dim[3] = 480;
    dim[4] = 40;
    dim[5] = 90;
}

XnCommunication::XnCommunication(string ip, int port) : clientIp(ip), clientPort(port) {
    /*
     *for (int i = 1; i < 6; i++) {
     *    trackedHand.push_back(false);
     *}
     *handBuffer.push(trackedHand);
     */
    dim[0] = 0;
    dim[1] = 640;
    dim[2] = 0;
    dim[3] = 480;
    dim[4] = 40;
    dim[5] = 90;
}

void XnCommunication::composeJsonMsg(float leader[]) {
    std::ostringstream ss;
    //ss << "{\"id\":" << 0 << ", \"state\":" << 0 << ", \"X\":" << leader[0]/dim[1] << ", \"Y\":" << leader[1]/dim[3] << ", \"Z\":" << leader[2] << "}";
    ss << "{\"leader\": {\"id\":" << 0 << ", \"state\":" << 0 << ", \"X\":" << leader[0]/dim[1] << ", \"Y\":" << leader[1]/dim[3] << ", \"Z\":" << leader[2] << "}}";
    socketServer << ss.str();
    std::cout << ss.str() << std::endl;
}

void XnCommunication::composeJsonMsg(XnVHandTracker* Blob, float leader[]) {
    /*
     *trackedHand.clear();
     *for (int i = 1; i < 6; i++) {
     *    trackedHand.push_back(false);
     *}
     *trackedHand[0] = true;
     */
    std::ostringstream ss;
    ss << "{";
    for (int i = 0; i < Blob->Fingers.size(); i++ ) {
        ss << "\"blob" << i << "\": {";
        /*
         *trackedHand[Blob->Fingers[i].id] = true;
         *if ( handBuffer.front()[Blob->Fingers[i].id] ) {
         *    std::cout << "Updating cursor " << Blob->Fingers[i].id << std::endl;
         *    Blob->Fingers[i].state = 0;
         *}
         *else {
         *    Blob->Fingers[i].state = 1;
         *    std::cout << "Creating cursor " << Blob->Fingers[i].id << std::endl;
         *}
         */
        Blob->Fingers[i].state = 0;     // for test, to delete considering above code
        ss << "\"id\":" << Blob->Fingers[i].id << ", \"state\":" << Blob->Fingers[i].state << ", \"X\":" << Blob->Fingers[i].coordinates.X / dim[1] << ", \"Y\":" << Blob->Fingers[i].coordinates.Y / dim[3] << ", \"Z\":" << (Blob->Fingers[i].coordinates.Z - dim[4])/(dim[5] - dim[4]) << "}";
        if ( i != Blob->Fingers.size()-1 ) 
            ss << ",";
    }
    ss << "}";
    socketServer << ss.str();
    std::cout << ss.str() << std::endl;
    /*
     *for (int i=0; i < 6; i++) {
     *    std::ostringstream ss;
     *    if ( !trackedHand[i] && handBuffer.front()[i] ) {
     *        std::cout << "Deleting cursor " << i << std::endl;
     *        ss << "{\"id\":" << i << ", \"state\":" << -1 << std::endl; 
     *        socketServer << ss.str();
     *        std::cout << ss.str() << std::endl;
     *    }
     *}
     *handBuffer.push(trackedHand);
     *handBuffer.pop();
     */
}

void XnCommunication::shutdown() {
    std::ostringstream ss;
    ss << ";";
    socketServer << ss.str();
}

