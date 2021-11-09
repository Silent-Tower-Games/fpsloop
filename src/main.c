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

clock_t before;
clock_t after;
int main()
{
    printf("Hello, World!\n");
    
    int runTimes = 5;
    int amount[runTimes];
    int average = 0;
    
    int delta;
    int deltaMS;
    
    FPSLoop fps = FPSLoop_Create(FPSLOOP_TYPE_BURNCPU, 60, frame);
    
    for(int i = 0; i < runTimes; i++)
    {
        frameCounter = 30;
        
        before = clock();
        FPSLoop_Run(fps);
        after = clock();
        
        assert(frameCounter == 0);
        
        delta = after - before;
        deltaMS = (delta * 1000) / CLOCKS_PER_SEC;
        
        printf("That took %d (%d ms)\n", delta, deltaMS);
        
        amount[i] = deltaMS;
        average += deltaMS;
    }
    
    average /= runTimes;
    
    printf("Average MS: %d\n", average);
    
    return 0;
}
