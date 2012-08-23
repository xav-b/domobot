#include "XEventsEmulation.h"

using namespace std;

XEventsEmulation::XEventsEmulation() {
    for (int i = 0; i < AVERAGE_BUFFER; i++) {
        xCoordinatesBuffer[i] = 0;
        yCoordinatesBuffer[i] = 0;
    }
}

void XEventsEmulation::calibrateScreen() {
    /*
     *Calibrate an appropriate relation between user movement and cursor's one
     */
}

int XEventsEmulation::smoothingPoint(int point, char axe) {
    /*
     *Filter user's hand coordinates
     */
    int average(0);
    if ( axe == 'x' ) {
        for (int i = 0; i < AVERAGE_BUFFER-1; i++)  {
            average += xCoordinatesBuffer[i];
            xCoordinatesBuffer[i] = xCoordinatesBuffer[i+1];
        }
        xCoordinatesBuffer[AVERAGE_BUFFER-1] = SCALE * (point + X_OFFSET);
        return (((average + xCoordinatesBuffer[AVERAGE_BUFFER-1]) / AVERAGE_BUFFER));
    }
    else if ( axe == 'y' ) {
        for (int i = 0; i < AVERAGE_BUFFER-1; i++)  {
            average += yCoordinatesBuffer[i];
            yCoordinatesBuffer[i] = yCoordinatesBuffer[i+1];
        }
        yCoordinatesBuffer[AVERAGE_BUFFER-1] = SCALE * (Y_OFFSET - point);
        return (((average + yCoordinatesBuffer[AVERAGE_BUFFER-1]) / AVERAGE_BUFFER));
    }
    return -1;    
}

int XEventsEmulation::mouseClick(string button, string type) {
    int value_r(-1);
    string xte_cmd = "xte 'mouse" + type + " " + button + "'";
    if (system(NULL)) {
        //cout << "[DEBUG] OK - Running xte action: " + xte_cmd + "\n";
        value_r = system( xte_cmd.c_str() );
        //cout << "[DEBUG] Returned value: " << value_r << endl;
    }
    else
        value_r = 2;
    return value_r;
}

int XEventsEmulation::mouseMove(int x, int y) {
    int value_r(-1);
    string xte_cmd = "xte 'mousemove ";

    ostringstream oss;
    oss << x << " " << y;
    string coordinates = oss.str();
    xte_cmd = xte_cmd + coordinates + "'";

    if (system(NULL)) {
        //cout << "[DEBUG] OK - Running mouse movement: " + xte_cmd + "\n";
        value_r = system( xte_cmd.c_str() );
        //cout << "[DEBUG] Returned value: " << value_r << endl;
    }
    else
        return 2;
    return value_r;
}

int XEventsEmulation::keyHit(string key, string type) {
    int value_r(-1);
    string xte_cmd = "xte 'key" + type + " " + key + "'";
    if (system(NULL)) {
        //cout << "[DEBUG] OK - Running mouse movement: " + xte_cmd + "\n";
        value_r = system( xte_cmd.c_str() );
        //cout << "[DEBUG] Returned value: " << value_r << endl;
    }
    else
        return 2;
    return value_r;
}
