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


void deflatten_3d(const int i, const int Ni, const int Nj, const int Nk, int* coord);

void bubbleSortList(int **list, int l);

#endif /* defined(__networkgen__ArrayFunctions__) */
