#include <stdio.h>
#include <SDL2/SDL.h>
#include "FPSLoop.h"

int main()
{
    printf("Hello, World!\n");
    
    FPSLoop fps = FPSLoop_Create(FPSLOOP_TYPE_BURNCPU, 1, NULL);
    
    FPSLoop_Run(fps);
    
    return 0;
}
