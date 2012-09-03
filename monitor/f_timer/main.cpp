#include <cstdio>
#include <iostream>
#include "Profiler.h"
#include <time.h>
#include <sys/resource.h>
#include <cmath>

using namespace std;

int main() {
    Profiler profiler;

    sandGlass chrono = profiler();
    cout << "Global: " << chrono.globalTimer << "s\n";
    cout << "Relative: " << chrono.globalRelativeTimer << "s\n\n";

    float useless(0);
    for (int i = 0; i < 100000000; i++)
        useless = pow(i, 2); 

    chrono = profiler();
    cout << "Global: " << chrono.globalTimer << "s\n";
    cout << "Relative: " << chrono.globalRelativeTimer << "s\n\n";

    for (int i = 0; i < 100000000; i++)
        useless = pow(i, 2); 

    chrono = profiler();
    cout << "Global: " << chrono.globalTimer << "s\n";
    cout << "Relative: " << chrono.globalRelativeTimer << "s\n\n";

    return 0;
}
