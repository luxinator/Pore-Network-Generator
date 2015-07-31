//
//  ArrayFunctions.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "ArrayFunctions.h"

#define MAX_STACK 64 /* Max Stacksize quicksort  Could use a C++ stack */

/*
 * Bubble sort is slow, but its a stable sort and easy to understand
 */
void bubbleSortList(int **list, const size_t l){
    
    std::cout<< "\t Bubble Sorting " << l << " values" << std::endl;
    bool sorted = false;
    int t1, t2; // temp values
    std::size_t passes = 0;
    while (!sorted){
    	passes++;
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
    std::cout << "\t Passes: " << passes << std::endl;
}

void bubbleSortList(int **list, const int l){
    bubbleSortList(list, l);
}


template <typename T> void QuickSortList(T **list, size_t len) {

    size_t left = 0, stack[MAX_STACK], pos = 0, seed = rand();
    while(true) {                                           /* outer loop */
        for (; left + 1 < len; len++) {                /* sort left to len-1 */
            if (pos == MAX_STACK) len = stack[pos = 0];  /* stack overflow, reset */
            T pivot = list[0][left + seed % (len - left)];  /* pick random pivot */
            seed = seed * 69069 + 1;                /* next pseudorandom number */
            stack[pos++] = len;                    /* sort right part later */
            for (size_t right = left - 1; ;) { /* inner loop: partitioning */
                while (list[0][++right] < pivot);  /* look for greater element */
                while (pivot < list[0][--len]);    /* look for smaller element */
                if (right >= len) break;           /* partition point found? */

                T temp = list[0][right];
                list[0][right] = list[0][len];                  /* the only swap */
                list[0][len] = temp;

                temp = list[1][right];
                list[1][right] = list[1][len];                  /* the only swap */
                list[1][len] = temp;

            }                            /* partitioned, continue left part */

            if (pos == 0) break;                               /* stack empty? */
            left = len;                             /* left to right is sorted */
            len = stack[--pos];                      /* get next range to sort */
        }
    }

}
