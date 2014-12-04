//
//  ArrayFunctions.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "ArrayFunctions.h"


int *** generate_naive_array(const int Ni, const int Nj, const int Nk){
    
    
    int i,j,k;
    //  Allocate 3D Array, Yess Pointer->Pointer->Pointer->int ...
    // continous block needs some work, I do not understand it probably
    int ***porebodies = new int**[Ni];
    
    for(i = 0; i < Ni; i++){
        porebodies[i] = new int*[Nj];
        for(j = 0; j < Nj; j++){
            porebodies[i][j] = new int[Nk];
            for(k = 0; k < Nk; k++){
                porebodies[i][j][k] = i * Nj * Nk + j * Nk + k + 1;
            }
        }
    }
    return porebodies;
}

void deflatten_3d(const int i, const int Ni, const int Nj, const int Nk, int* coord){
    
    //Since it is not really nice to return stack memory we malloc on the heap.

    coord[0] = (i / (Nj * Nk)) % Ni;
    coord[1] = (i / Nk) % Nj;
    coord[2] = i % Nk;
    
}

