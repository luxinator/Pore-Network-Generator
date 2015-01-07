//
//  ArrayFunctions.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__ArrayFunctions__
#define __networkgen__ArrayFunctions__

#include <iostream>
#include <vector>

/*
 * Insert pb nr and returns the lattice coordinates in the coord array
 */
inline void deflatten_3d(size_t i, const int Ni, const int Nj, const int Nk, int* coord){
    i -= 1;
    coord[0] = (i / (Nj * Nk)) % Ni;
    coord[1] = (i / Nk) % Nj;
    coord[2] = i % Nk;
    
}

void bubbleSortList(int **list, size_t l);
void bubbleSortList(int **list, int l);

#endif /* defined(__networkgen__ArrayFunctions__) */
