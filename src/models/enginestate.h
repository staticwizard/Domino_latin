#ifndef ENGINESTATE_H
#define ENGINESTATE_H

#include <stdbool.h>

enum states {
    s_readyForStart, 
    s_gameStarted, 
    s_gameEnded, 
    s_setStarted, 
    s_setEnded, 
    s_setBlocked }; 

typedef struct _EngineState {
    bool gameStarted;
    bool gameEnded;
    bool setStarted;
    bool setEnded;
    bool setBlocked;
    bool playing;
    
    enum states currentState;
    
} EngineState;

//enums for the engine states


#endif