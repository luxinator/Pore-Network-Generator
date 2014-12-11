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

    coord[0] = (i / (Nj * Nk)) % Ni;
    coord[1] = (i / Nk) % Nj;
    coord[2] = i % Nk;
    
}

void bubbleSortList(int **list, const int l){
    
    std::cout<< "\t Bubble Sorting " << l << " values" << std::endl;
    bool sorted = false;
    int t1, t2; // temp values
    while (!sorted){
        sorted = true;
    
        for(int i = 0; i < l - 1; i++){
            if (list[0][i] > list[0][i+1]) {
                sorted = false;
                //if (list[1][i] == 0 || list[0][i] == 0
                //    || list[1][i+1] == 0 || list[0][i+1] == 0) {
                //    std::cout<<"BS!"<<std::endl;
                //    std::cout<<i<< std::endl;
                //}
                
                t1 = list[0][i];
                t2 = list[1][i];
                
                list[0][i] = list[0][i+1];
                list[1][i] = list[1][i+1];
                
                list[0][i+1] = t1;
                list[1][i+1] = t2;

            }
        }
    }
}
