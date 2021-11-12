#pragma once

#include <SDL2/SDL.h>

/**
 * Ways for FPSLoop to keep time
 */
typedef enum FPSLoop_Type
{
    // invalid
    FPSLOOP_TYPE_INVALID = -1,
    // checks the time each tick
    FPSLOOP_TYPE_BURNCPU,
    // expects vsync to wait between frames, does no timing work
    FPSLOOP_TYPE_VSYNC,
    // sleeps for the remainder of the frame
    FPSLOOP_TYPE_SLEEP,
    // sleeps for small periods & checks if it has overslept
    FPSLOOP_TYPE_SLEEPSMART,
} FPSLoop_Type;

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

/**
 * Create an FPSLoop instance
 * 
 * \param type how the FPS timer should work
 * \param FPS how many frames per second your timer should run at
 * 
 * \returns FPSLoop instance
 */
FPSLoop FPSLoop_Create(FPSLoop_Type type, int FPS, int (*frame)());

/**
 * Run your FPSLoop application
 * 
 * \param fps your FPSLoop instance
 */
void FPSLoop_Run(FPSLoop fps);
