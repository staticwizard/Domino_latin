#ifndef FACT_H
#define FACT_H

#include <stdint.h>

enum Fact_Type { STRENGTH, WEAKNESS, PREDICTION
};

typedef struct _Fact 
{
    int32_t playerPos;  //fact about a player in position playerPos
    float certainty;    //the certainty of a fact
    enum Fact_Type type;
}Fact;

#endif
