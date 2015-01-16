//
//  ArrayFunctions.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "ArrayFunctions.h"


//kjkjk



/*
 * Bubble sort is slow, but its a stable sort and easy to understand
 */
void bubbleSortList(int **list, const size_t l){
    
    std::cout<< "\t Bubble Sorting " << l << " values" << std::endl;
    bool sorted = false;
    int t1, t2; // temp values
    while (!sorted){
        sorted = true;
    
        for(size_t i = 0; i < l - 1; i++){
            if (list[0][i] > list[0][i+1]) {
                sorted = false;
                
                //swap values
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

void bubbleSortList(int **list, const int l){
    bubbleSortList(list, l);
}
