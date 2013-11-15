#include <Poco/Runnable.h>
#include <Poco/Thread.h>
#include <iostream>

#include "utils.h"
#include "PocketVoice.cpp"

using namespace Poco;
using namespace std;

class MyWorker : public Runnable
{
    public:
        MyWorker(std::string gramConfig, std::string cmdConfig) : _gramConfig(gramConfig), _cmdConfig(cmdConfig), Runnable() {}

        virtual void run()
        {
            std::string said("");
            std::cout << "PocketVoice test\n";
            PocketVoice* recognizer = new PocketVoice;

            recognizer->loadConfigCommands(_cmdConfig);

            //if ( recognizer->init("config.es", _gramConfig) > 0 ) 
            if ( recognizer->init("", _gramConfig) > 0 ) 
                fatal("[main()] initiating recognizer");

            recognizer->calibrateListener();

            while (said != "goodbye") {
                said = recognizer->recognize();

                if ( recognizer->synthetize(said) > 0 ) 
                    fatal("[main()] synthetizing voice");

                recognizer->execute(said);
            }

            delete recognizer;
        }

    private:
        std::string _gramConfig;
        std::string _cmdConfig;
};

int main(int argc, char** argv)
{
    if ( argc < 2 ) 
        return -1;

    MyWorker w = MyWorker(argv[1], argv[2]);

    Thread t;
    t.start(w);
    std::cout << "************************** Debug **************************\n";
    t.join();  // wait for all threads to end

    cout << endl << "Threads joined" << endl;

    return 0;
}
