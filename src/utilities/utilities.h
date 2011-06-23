/**
 * @Author: W.k.Tse
 * @Date : 14-2-'11
 * @Description: Helper utilities
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>

//returns a random number that is between 0 and pMax non-inclusive.
//will return 0 <-> pMax -1
/**
 * Return a random number between 0 and pMax (non-inclusive)
 * @pre pMax is larger than 0
 * @post a random number is returned between 0 (inclusive) and pMax(non-inclusive)
 * @return a random number
 */
int32_t getRand (int32_t pMax);

/** 
 * reseed the random generator
 */
void reseed();

#endif