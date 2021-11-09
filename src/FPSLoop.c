#include <assert.h>
#include <SDL2/SDL.h>
#include "FPSLoop.h"

FPSLoop FPSLoop_Create(FPSLoop_Type type, int FPS, int (*frame)())
{
    assert(frame != NULL);
    
    return (FPSLoop){
        .type = type,
        .FPS = FPS,
        .frame = frame,
    };
}

static void FPSLoop_Run_Vsync(int FPS, int (*frame)())
{
    while(!frame()){}
}

static void FPSLoop_Run_BurnCPU(int FPS, int (*frame)())
{
    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 currentTime;
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 deltaTime;
    int64_t accumulator = 0;
    Uint64 FPSMS = freq / FPS;
    
    int quit = 0;
    
    while(!quit)
    {
        currentTime = SDL_GetPerformanceCounter();
        
        deltaTime = currentTime - lastTime;
        
        accumulator += deltaTime;
        
        lastTime = currentTime;
        
        while(accumulator > FPSMS)
        {
            //printf("%" PRIu64 " ? %" PRIu64 "\n", accumulator, FPSMS);
            
            quit = frame();
            
            if(quit)
            {
                break;
            }
            
            accumulator -= FPSMS;
        }
    }
}

void FPSLoop_Run(FPSLoop fps)
{
    switch(fps.type)
    {
        case FPSLOOP_TYPE_VSYNC:
        {
            FPSLoop_Run_Vsync(fps.FPS, fps.frame);
        } break;
        
        case FPSLOOP_TYPE_BURNCPU:
        {
            FPSLoop_Run_BurnCPU(fps.FPS, fps.frame);
        } break;
        
        default:
        {
            assert(0);
        } break;
    }
}
