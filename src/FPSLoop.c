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

void FPSLoop_Run(FPSLoop fps)
{
    switch(fps.type)
    {
        case FPSLOOP_TYPE_VSYNC:
        {
            FPSLoop_Run_Vsync(fps.FPS, fps.frame);
        } break;
    }
}
