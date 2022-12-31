#include <inttypes.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "FPSLoop.h"

#ifndef FPSLOOPTYPE
#define FPSLOOPTYPE FPSLOOP_TYPE_SLEEPSMART
#endif

SDL_Window* window;
SDL_Renderer* renderer;
int frameCounter;
char frame()
{
    frameCounter--;
    
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    if(frameCounter <= 0)
    {
        return 1;
    }
    
    return 0;
}

int main(int argc, char** argv)
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
    
    FPSLoop_Type loopType = FPSLOOPTYPE;
    FPSLoop* fps = FPSLoop_Create(loopType, 60, frame);
    
    printf("Using loop type \"%s\"\n", FPSLoop_GetLoopTypeString(fps));
    
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
        // We want to turn on Vsync if we're usuing the no-timing loop
        // So that it doesn't all run at once
        loopType == FPSLOOP_TYPE_NOTHING ? SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_ACCELERATED
    );
    
    for(int i = 0; i < runTimes; i++)
    {
        frameCounter = 60;
        
        before = SDL_GetPerformanceCounter();
        FPSLoop_Run(fps);
        after = SDL_GetPerformanceCounter();
        
        delta = after - before;
        deltaMS = (delta * 1000) / freq;
        
        printf("That took %" PRIu64 " (%" PRIu64 " ms) (Average frame: %1.2f)\n", delta, deltaMS, FPSLoop_GetAverageFPS(fps));
        
        amount[i] = deltaMS;
        average += deltaMS;
    }
    
    average /= runTimes;
    
    printf("Average MS: %d\n", average);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    FPSLoop_Destroy(fps);
    
    return 0;
}
