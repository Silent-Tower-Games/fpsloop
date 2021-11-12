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

static int FPSLoop_Frame(int (*frame)())
{
    int frameResult = frame();
    
    return frameResult;
}

static void FPSLoop_Run_Vsync(int FPS, int (*frame)())
{
    while(!FPSLoop_Frame(frame)){}
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
            quit = FPSLoop_Frame(frame);
            
            if(quit)
            {
                break;
            }
            
            accumulator -= FPSMS;
        }
    }
}

static void FPSLoop_Run_Sleep(int FPS, int (*frame)())
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
            quit = FPSLoop_Frame(frame);
            
            if(quit)
            {
                break;
            }
            
            accumulator -= FPSMS;
        }
        
        if(accumulator < FPSMS)
        {
            SDL_Delay((accumulator * 1000) / freq);
        }
    }
}

static void FPSLoop_Run_SleepSmart(int FPS, int (*frame)())
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
            quit = FPSLoop_Frame(frame);
            
            if(quit)
            {
                break;
            }
            
            accumulator -= FPSMS;
        }
        
        // Sleep for 1ms if our target time is more than 1ms away
        if(accumulator < FPSMS - (1000 / freq))
        {
            SDL_Delay(1);
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
        
        case FPSLOOP_TYPE_SLEEP:
        {
            FPSLoop_Run_Sleep(fps.FPS, fps.frame);
        } break;
        
        case FPSLOOP_TYPE_SLEEPSMART:
        {
            FPSLoop_Run_SleepSmart(fps.FPS, fps.frame);
        } break;
        
        default:
        {
            assert(0);
        } break;
    }
}
