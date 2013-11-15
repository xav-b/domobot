#include "utils.h"
#include "PocketVoice.cpp"

int main(int argc, char* argv[])
{
    std::string said("");
    std::cout << "PocketVoice test\n";
    PocketVoice* recognizer = new PocketVoice;

    recognizer->loadConfigCommands("commands.json");

    if ( recognizer->init("", "commands.gram") > 0 ) 
        fatal("[main()] initiating recognizer");

    recognizer->calibrateListener();

    said = recognizer->recognize();

    if ( recognizer->synthetize(said) > 0 ) 
        fatal("[main()] synthetizing voice");

    recognizer->execute(said);

    delete recognizer;
    return 0;
}
