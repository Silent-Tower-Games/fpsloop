#include <assert.h>
#include <SDL2/SDL.h>
#include "FPSLoop.h"

typedef struct FPSLoop
{
    // type of loop timing
    FPSLoop_Type type;
    // frames per second
    int FPS;
    // your app logic function. runs once per frame & returns a bool for whether or not to exit
    int (*frame)();
    // 
    //Uint64 
} FPSLoop;

FPSLoop* FPSLoop_Create(FPSLoop_Type type, int FPS, int (*frame)())
{
    assert(frame != NULL);
    
    FPSLoop* ret = malloc(sizeof(FPSLoop));
    *ret = (FPSLoop){
        .type = type,
        .FPS = FPS,
        .frame = frame,
    };
    
    return ret;
}

char* FPSLoop_GetTypeString(FPSLoop* fps)
{
    return FPSLoop_GetTypeStringFromType(fps->type);
}

char* FPSLoop_GetTypeStringFromType(FPSLoop_Type type)
{
    switch(type)
    {
        case FPSLOOP_TYPE_BURNCPU:
        {
            return "Burn CPU";
        }
        case FPSLOOP_TYPE_NOTHING:
        {
            return "Nothing";
        }
        case FPSLOOP_TYPE_SLEEP:
        {
            return "Sleep";
        }
        case FPSLOOP_TYPE_SLEEPSMART:
        {
            return "Sleep Smart";
        }
    }
    
    return NULL;
}

static int FPSLoop_Frame(int (*frame)())
{
    int frameResult = frame();
    
    return frameResult;
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

static void FPSLoop_Run_Nothing(int FPS, int (*frame)())
{
    while(!FPSLoop_Frame(frame)){}
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
            SDL_Delay(((FPSMS - accumulator) * 1000) / freq);
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

void FPSLoop_Run(FPSLoop* fps)
{
    switch(fps->type)
    {
        case FPSLOOP_TYPE_NOTHING:
        {
            FPSLoop_Run_Nothing(fps->FPS, fps->frame);
        } break;
        
        case FPSLOOP_TYPE_BURNCPU:
        {
            FPSLoop_Run_BurnCPU(fps->FPS, fps->frame);
        } break;
        
        case FPSLOOP_TYPE_SLEEP:
        {
            FPSLoop_Run_Sleep(fps->FPS, fps->frame);
        } break;
        
        case FPSLOOP_TYPE_SLEEPSMART:
        {
            FPSLoop_Run_SleepSmart(fps->FPS, fps->frame);
        } break;
        
        default:
        {
            assert(0);
        } break;
    }
}

void FPSLoop_Destroy(FPSLoop* fps)
{
    // Simple as that!
    free(fps);
}
