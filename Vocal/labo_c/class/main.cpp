#include "PocketVoice.cpp"

int main(int argc, char* argv[])
{
    std::cout << "PocketVoice test\n";
    PocketVoice* recognizer = new PocketVoice;
    if ( recognizer->init("", "commands.gram") > 0 ) {
        std::cout << "** Failed to initialize recognizer\n";
        return 1;
    }
    recognizer->calibrateListener();
    recognizer->recognize();
    delete recognizer;

    return 0;
}
