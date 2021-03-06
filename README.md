# FPSLoop

A good & simple gameplay loop for C and SDL2.

## Dependencies

- A C compiler
- SDL2

## Building

The easiest way is just to run `make`, which will build the shared lib & the test program. There are other methods in the Makefile as well.

## Example

```c
// Return 1 to exit, 0 to keep going
int frame()
{
    // Keep going forever
    return 0;
}

int main()
{
    FPSLoop* fps = FPSLoop_Create(
        // How should the app keep time?
        FPSLOOP_TYPE_SLEEPSMART,
        // How many frames per second?
        60,
        // Pointer to the function that holds your game logic
        frame
    );
    
    // Your application will run inside this function
    // It will stop running when your logic function has decided to close
    FPSLoop_Run(fps);
    
    // Free the FPSLoop* memory
    FPSLoop_Destroy(fps);
    
    return 0;
}
```

## Loop Types

You can change the Loop Type mid-application:

```c
FPSLoop_SetLoopType(fps, FPSLOOP_TYPE_SLEEP); // replace _SLEEP with your desired loop type
```

The enum `FPSLoop_Type` contains several values:

### BURNCPU

This type should have the closest thing to perfect timing, at the cost of a lot of CPU power.

### SLEEP

This type will use the least CPU power, but will also potentially sacrifice accurate timing.

### SLEEPSMART

This type combines `BURNCPU` and `SLEEP` to use a little more CPU power & get a lot more accuracy.

### NOTHING

This type does nothing for timing whatsoever. This might be useful if you explicitly use vsync, but also it really might not.

## Other Methods

### GetAverageFPS

`float FPSLoop_GetAverageFPS(FPSLoop*)` will return a `float` value of the average of the last 120 frames (or a different number, if given a compiler flag for `FPSLOOP_FRAME_PERFORMANCE_COUNT`).

### GetLoopTypeString

`char* FPSLoop_GetLoopTypeString(FPSLoop*)` will return a string denoting which loop type is being used for the given `FPSLoop` instance.

### GetLoopTypeStringFromType

`char* FPSLoop_GetLoopTypeStringFromType(FPSLoop_Type)` will return a string corresponding to the given `FPSLoop_Type` value.
