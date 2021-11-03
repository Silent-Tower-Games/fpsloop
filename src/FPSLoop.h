#pragma once

typedef enum FPSLoop_Type
{
    FPSLOOP_TYPE_INVALID = -1,
    FPSLOOP_TYPE_BURNCPU,
    FPSLOOP_TYPE_VSYNC,
    FPSLOOP_TYPE_SLEEP,
    FPSLOOP_TYPE_SLEEP_SMART,
} FPSLoop_Type;

typedef struct FPSLoop
{
    /** type of loop timing */
    FPSLoop_Type type;
    /** frames per second */
    int FPS;
    /** function that runs once per frame & returns a bool for whether or not to exit */
    int (*frame)();
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
