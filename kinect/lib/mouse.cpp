#include <iostream>
#include "XEventsEmulation.cpp"

using namespace std;

int main() {
    /*
     * Crée le noyau manageant la configuration requise
     */
    XEventsEmulation interface;
    int x(200);
    int y(100);
    int new_x(0);
    int new_y(0);
    /*
     * Lecture du fichier de configuration et paramétrage du process
     */
    if ( interface.mouseClick("3") < 0 )
        cout << "[ERROR] Clicking\n";
    if ( (new_x = interface.smoothingPoint(x, 'x')) < 0 )
        cout << "[ERROR] Smoothing x\n";
    if ( (new_y = interface.smoothingPoint(y, 'y')) < 0 )
        cout << "[ERROR] Smoothing y\n";
    if ( interface.mouseMove(new_x, new_y) < 0 )
        cout << "[ERROR] Moving\n";
    return 0;
}
