#include "GlutManager.cpp"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Go !\n";
    GlutManager glApp;
    glApp.mainLoop(argc, argv);
    return 0;
}
