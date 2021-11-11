#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "FPSLoop.h"

int frameCounter;
int frame()
{
    frameCounter--;
    
    if(frameCounter <= 0)
    {
        return 1;
    }
    
    return 0;
}

int main()
{
    printf("Hello, World!\n");
    
    int runTimes = 5;
    Uint64 amount[runTimes];
    int average = 0;
    
    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 before;
    Uint64 after;
    Uint64 delta;
    Uint64 deltaMS;
    
    FPSLoop fps = FPSLoop_Create(FPSLOOP_TYPE_BURNCPU, 60, frame);
    
    for(int i = 0; i < runTimes; i++)
    {
        frameCounter = 30;
        
        before = SDL_GetPerformanceCounter();
        FPSLoop_Run(fps);
        after = SDL_GetPerformanceCounter();
        
        assert(frameCounter == 0);
        
        delta = after - before;
        deltaMS = (delta * 1000) / freq;
        
        printf("That took %" PRIu64 " (%" PRIu64 " ms)\n", delta, deltaMS);
        
        amount[i] = deltaMS;
        average += deltaMS;
    }
    
    average /= runTimes;
    
    printf("Average MS: %d\n", average);
    
    return 0;
}
