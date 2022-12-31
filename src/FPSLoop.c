#include <assert.h>
#include <SDL2/SDL.h>
#include "FPSLoop.h"

#ifndef FPSLOOP_FRAME_PERFORMANCE_COUNT
// How many frames do we want to keep for our average?
#define FPSLOOP_FRAME_PERFORMANCE_COUNT 120
#endif

#ifndef FPSLOOP_SLEEP_PERFORMANCE_COUNT
// How many sleeps do we want to keep for our average?
#define FPSLOOP_SLEEP_PERFORMANCE_COUNT 120
#endif

typedef struct FPSLoop
{
    // type of loop timing
    FPSLoop_Type type;
    // frames per second
    int FPS;
    // your app logic function. runs once per frame & returns a bool for whether or not to exit
    char (*frame)();
    // time spent on the last X frames
    Uint64 frameTiming[FPSLOOP_FRAME_PERFORMANCE_COUNT];
    // average time spent on a frame over the last X frames
    Uint64 frameAverage;
    // time spent on the last X sleeps
    Uint64 sleepTiming[FPSLOOP_SLEEP_PERFORMANCE_COUNT];
    // number of sleeps counted so far. caps at FPSLOOP_SLEEP_PERFORMANCE_COUNT
    Uint64 sleepHighest;
} FPSLoop;

FPSLoop* FPSLoop_Create(FPSLoop_Type type, int FPS, char (*frame)())
{
    assert(frame != NULL);
    
    FPSLoop* ret = calloc(1, sizeof(FPSLoop));
    ret->type = type;
    ret->FPS = FPS;
    ret->frame = frame;
    
    return ret;
}

float FPSLoop_GetAverageFPS(FPSLoop* fps)
{
    const Uint64 freq = SDL_GetPerformanceFrequency();
    
    return ((freq * 1000) / fps->frameAverage) / 1000.0f;
}

char* FPSLoop_GetLoopTypeString(FPSLoop* fps)
{
    return FPSLoop_GetLoopTypeStringFromType(fps->type);
}

char* FPSLoop_GetLoopTypeStringFromType(FPSLoop_Type type)
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

void FPSLoop_SetLoopType(FPSLoop* fps, FPSLoop_Type type) 
{
    assert(fps != NULL);
    // So you can't change to an invalid loop type
    assert(FPSLoop_GetLoopTypeStringFromType(type) != NULL);

    fps->type = type;
}

static int FPSLoop_Frame(FPSLoop* fps)
{
    int frameResult = fps->frame();
    
    Uint64 current = SDL_GetPerformanceCounter();
    
    // This loop accomplishes two things:
    // - move frames 1-through-X-minus-1 to 0-through-X-minus-2
    // - gets a sum of all frame counts (starting with 1, not 0)
    fps->frameAverage = 0;
    for(int i = 1; i < FPSLOOP_FRAME_PERFORMANCE_COUNT; i++)
    {
        fps->frameAverage += fps->frameTiming[i] - fps->frameTiming[i - 1];
        
        fps->frameTiming[i - 1] = fps->frameTiming[i];
    }
    
    fps->frameTiming[FPSLOOP_FRAME_PERFORMANCE_COUNT - 1] = current;
    
    fps->frameAverage += fps->frameTiming[FPSLOOP_FRAME_PERFORMANCE_COUNT - 1] - fps->frameTiming[FPSLOOP_FRAME_PERFORMANCE_COUNT - 2];
    fps->frameAverage /= FPSLOOP_FRAME_PERFORMANCE_COUNT;
    
    return frameResult;
}

static void FPSLoop_Run_BurnCPU(FPSLoop* fps)
{
    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 currentTime;
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 deltaTime;
    int64_t accumulator = 0;
    Uint64 FPSMS = freq / fps->FPS;
    
    int quit = 0;
    
    while(!quit)
    {
        currentTime = SDL_GetPerformanceCounter();
        
        deltaTime = currentTime - lastTime;
        
        accumulator += deltaTime;
        
        lastTime = currentTime;
        
        while(accumulator > FPSMS)
        {
            quit = FPSLoop_Frame(fps);
            
            if(quit)
            {
                break;
            }
            
            accumulator -= FPSMS;
        }
    }
}

static void FPSLoop_Run_Nothing(FPSLoop* fps)
{
    while(!FPSLoop_Frame(fps)){}
}

static void FPSLoop_Run_Sleep(FPSLoop* fps)
{
    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 currentTime;
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 deltaTime;
    int64_t accumulator = 0;
    Uint64 FPSMS = freq / fps->FPS;
    
    int quit = 0;
    
    while(!quit)
    {
        currentTime = SDL_GetPerformanceCounter();
        
        deltaTime = currentTime - lastTime;
        
        accumulator += deltaTime;
        
        lastTime = currentTime;
        
        while(accumulator > FPSMS)
        {
            quit = FPSLoop_Frame(fps);
            
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

static void FPSLoop_Run_SleepSmart(FPSLoop* fps)
{
    const Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 currentTime;
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 deltaTime;
    int64_t accumulator = 0;
    Uint64 FPSMS = freq / fps->FPS;
    
    int quit = 0;
    
    while(!quit)
    {
        currentTime = SDL_GetPerformanceCounter();
        
        deltaTime = currentTime - lastTime;
        
        accumulator += deltaTime;
        
        lastTime = currentTime;
        
        while(accumulator > FPSMS)
        {
            quit = FPSLoop_Frame(fps);
            
            if(quit)
            {
                break;
            }
            
            accumulator -= FPSMS;
        }
        
        // Sleep for 1ms if our target time is more than sleepHighest away
        if(accumulator < FPSMS - fps->sleepHighest)
        {
            const Uint64 before = SDL_GetPerformanceCounter();
            SDL_Delay(1);
            const Uint64 after = SDL_GetPerformanceCounter();
            
            fps->sleepHighest = 0;
            for(int i = 1; i < FPSLOOP_SLEEP_PERFORMANCE_COUNT; i++)
            {
                if(fps->sleepTiming[i] > fps->sleepHighest)
                {
                    fps->sleepHighest = fps->sleepTiming[i];
                }
                
                fps->sleepTiming[i - 1] = fps->sleepTiming[i];
            }
            
            fps->sleepTiming[FPSLOOP_SLEEP_PERFORMANCE_COUNT - 1] = after - before;
            
            if(fps->sleepTiming[FPSLOOP_SLEEP_PERFORMANCE_COUNT - 1] > fps->sleepHighest)
            {
                fps->sleepHighest = fps->sleepTiming[FPSLOOP_SLEEP_PERFORMANCE_COUNT - 1];
            }
        }
    }
}

void FPSLoop_Run(FPSLoop* fps)
{
    switch(fps->type)
    {
        case FPSLOOP_TYPE_NOTHING:
        {
            FPSLoop_Run_Nothing(fps);
        } break;
        
        case FPSLOOP_TYPE_BURNCPU:
        {
            FPSLoop_Run_BurnCPU(fps);
        } break;
        
        case FPSLOOP_TYPE_SLEEP:
        {
            FPSLoop_Run_Sleep(fps);
        } break;
        
        case FPSLOOP_TYPE_SLEEPSMART:
        {
            FPSLoop_Run_SleepSmart(fps);
        } break;
    }
}

void FPSLoop_Destroy(FPSLoop* fps)
{
    // Simple as that!
    free(fps);
}
