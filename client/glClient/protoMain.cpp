#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "networkHandler.cpp"

int main(int argc, char* argv[])
{
    std::cout << "main: startup" << std::endl;
    networkHandler nh(10002);
    nh.init();
    nh.start();
    std::cout << "main: waiting for thread" << std::endl;
    nh();
    if ( nh.running() )
        std::cout << "Process running\n";
    nh.join();
    std::cout << "main: done" << std::endl;
    return 0;
}

