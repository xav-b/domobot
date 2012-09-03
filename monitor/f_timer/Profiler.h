#ifndef DEF_PROCESS_CHRONO
#define DEF_PROCESS_CHRONO

//#include <vector>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>

typedef struct { 
    float globalRelativeTimer;
    float globalTimer;
} sandGlass;

class Profiler {
    public:
        Profiler() 
        {
            struct timespec ts;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
            initChrono = (float)ts.tv_sec + ((float)ts.tv_nsec / 1000000000);
            chrono.globalTimer = 0;
            chrono.globalRelativeTimer = 0;
        }

        sandGlass operator()()
        {
            struct timespec ts;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
            chrono.globalRelativeTimer = ((float)ts.tv_sec + ((float)ts.tv_nsec / 1000000000)) - chrono.globalRelativeTimer;
            chrono.globalTimer = ((float)ts.tv_sec + ((float)ts.tv_nsec / 1000000000)) - initChrono;
            
            return chrono;
        }

    protected:
        sandGlass chrono;
        float initChrono;
};

#endif
