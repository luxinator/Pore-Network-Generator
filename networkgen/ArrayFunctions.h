//
//  ArrayFunctions.h
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __networkgen__ArrayFunctions__
#define __networkgen__ArrayFunctions__

#include <iostream>
#include <vector>

/*
 * Insert pb nr and returns the lattice coordinates
 */
inline void deflatten_3d(size_t i, const unsigned int Ni, const unsigned int Nj, const unsigned int Nk, int* coord){
    i -= 1;
    coord[0] = (i / (Nj * Nk)) % Ni;
    coord[1] = (i / Nk) % Nj;
    coord[2] = i % Nk;
    
}

//void bubbleSortList(int **list, size_t l);
//void bubbleSortList(int **list, int l);

void quicksort_iterative(int **array, size_t len);

#endif /* defined(__networkgen__ArrayFunctions__) */
