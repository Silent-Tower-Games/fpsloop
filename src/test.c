#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "FPSLoop.h"

SDL_Window* window;
SDL_Renderer* renderer;
int frameCounter;
int frame()
{
    frameCounter--;
    
    SDL_RenderPresent(renderer);
    
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
    
    FPSLoop_Type loopType = FPSLOOP_TYPE_SLEEPSMART;
    FPSLoop* fps = FPSLoop_Create(loopType, 60, frame);
    
    window = SDL_CreateWindow(
        "Test Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        100,
        100,
        0
    );
    renderer = SDL_CreateRenderer(
        window,
        -1,
        // We want to turn on VSync if we're usuing the VSync-oriented loop
        loopType == FPSLOOP_TYPE_VSYNC ? SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED
    );
    
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
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    return 0;
}
