#pragma once

/**
 * Ways for FPSLoop to keep time
 */
typedef enum FPSLoop_Type
{
    // checks the time each tick
    FPSLOOP_TYPE_BURNCPU,
    // does nothing for timing, just runs in a loop
    FPSLOOP_TYPE_NOTHING,
    // sleeps for the remainder of the frame
    FPSLOOP_TYPE_SLEEP,
    // sleeps for small periods & checks if it has overslept
    FPSLOOP_TYPE_SLEEPSMART,
} FPSLoop_Type;

typedef struct FPSLoop FPSLoop;

/**
 * Create an FPSLoop instance
 * 
 * \param type how the FPS timer should work
 * \param FPS how many frames per second your timer should run at
 * 
 * \returns pointer to your FPSLoop instance
 */
FPSLoop* FPSLoop_Create(FPSLoop_Type type, int FPS, int (*frame)());

/**
 * Get the average framerate from a given FPSLoop instance
 * 
 * \param fps pointer to your FPSLoop instance
 * 
 * \return average FPS
 */
float FPSLoop_GetAverageFPS(FPSLoop* fps);

/**
 * Get a string denoting what type of loop timing you're using
 * 
 * \param fps pointer to your FPSLoop instance
 * 
 * \return type string
 */
char* FPSLoop_GetLoopTypeString(FPSLoop* fps);

/**
 * Get a string denoting what type of loop timing you're using
 * 
 * \param type loop type
 * 
 * \return type string
 */
char* FPSLoop_GetLoopTypeStringFromType(FPSLoop_Type type);

/**
 * Run your FPSLoop application
 * 
 * \param fps pointer to your FPSLoop instance
 */
void FPSLoop_Run(FPSLoop* fps);

/**
 * Free your FPSLoop memory
 * 
 * \param fps pointer to your FPSLoop instance
 */
void FPSLoop_Destroy(FPSLoop* fps);
