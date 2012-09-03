#include <cstdlib>
#include <iostream>

int main() {
    int rc = system("python ./pySVM/fingerSVC.py");
    std::cout << "Process returned: " << rc << std::endl;
    return 0;
}
