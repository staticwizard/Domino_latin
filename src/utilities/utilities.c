/**
 * @Author: W.k.Tse
 * @Date : 14-2-'11
 * @Description: Helper utilities
 */

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "utilities.h"
#include <assert.h>
#include <stdio.h>


int getRand (int pMax) {

    assert (pMax > 0);
    
    static int seeded = 0;
    
    if (!seeded) { 
        long ltime = time (NULL);
        srand (ltime);
        seeded = 1;
    }
    return rand() % pMax;
}

void reseed() {
    long ltime = time (NULL);
    srand (ltime);
}
