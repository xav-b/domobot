#include <map>
#include <vector>

using namespace boost::property_tree;

//typedef struct 
//{
    //int id;
    //int state;
    //float xAngle;
    //float yAngle;
    //float zAngle;
//} Blob;
std::vector< std::map<std::string, float> > hand(6);

class networkHandler
{
public: 
    networkHandler(int p) : _port(p) {
        _running = false;
    }
    void init() {
        int yes(1);
        _server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
        setsockopt(_server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        _server_address.sin_family = AF_INET;
        _server_address.sin_port = htons(_port);
        _server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        memset(&(_server_address.sin_zero), '\0', 8);

        bind(_server_sockfd, (sockaddr*)&_server_address, sizeof(sockaddr));
        listen(_server_sockfd, 5);
        std::cout << "Init done\n";
    }
    void operator()()
    {
        if ( _running ) {
            std::cout << "Thread id: " << boost::this_thread::get_id() << std::endl; 
            std::cout << "Concurency: " << boost::thread::hardware_concurrency() << std::endl;
        } 
    }
    void start()
    {
        if ( !_running ) {
            _running = true;
            _Thread = boost::thread(&networkHandler::processConnection, this, _port);
        }
    }
    void join() 
    {
        _Thread.join();
    }
    bool running() {
        return _running;
    }
    void processConnection(int param) 
    {
        ptree pt;
        int connfd;
        std::cout << "Listening for client\n";
        char buf[1024];
        int recv_length = 1;
        try {
            connfd = accept(_server_sockfd, (sockaddr*)NULL, 0);
            std::cout << "Client connected\n";
            while (recv_length > 0) {
                int i(0);
                recv_length = recv(connfd, &buf, sizeof(buf), 0);
                buf[recv_length] = '\0';
                std::cout << buf << " - (" << recv_length << ")\n";
                if (buf[0] == ';') 
                    break; 
                //g_cursorsMutex.lock();
                std::stringstream ss;
                ss << buf;
                read_json(ss, pt);
                for ( ptree::iterator itRoot = pt.begin(); itRoot != pt.end(); itRoot++ ) {
                    ptree ptChild = pt.get_child(itRoot->first);
                    i++;
                    for ( ptree::iterator itChild = ptChild.begin(); itChild != ptChild.end(); itChild++ ) {
                        hand[i][itChild->first] = atof((itChild->second.data()).c_str());
                    }
                }
                //g_cursorsMutex.unlock();
            }
            //glutPostRedisplay();
            std::cout << "[I] Ending thread\n";
            close(connfd);
        } 
        catch (boost::thread_interrupted&) {}
        catch (std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }
    }

private:
    int _port;
    boost::thread _Thread;
    bool _running;
    int _server_sockfd;
    sockaddr_in _server_address;
};
