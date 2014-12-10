//
//  Generator.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__Generator__
#define __networkgen__Generator__


float **generateLocation(const float Length, int **throatCounters, const int Ni, const int Nj, const int Nk);

int **generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters);

int **generateFullConnectivity(const int Ni, const int Nj, const int Nk, int **halfConnectivity);

#endif /* defined(__networkgen__Generator__) */
