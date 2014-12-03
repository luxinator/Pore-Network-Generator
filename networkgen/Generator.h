//
//  Generator.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__Generator__
#define __networkgen__Generator__

#include <vector>
#include <tuple>


// Just a Wrapper.
typedef std::vector<std::pair<int, int>> connectionList;

float **generateLocation(const float Length, int **throatCounters, const int Ni, const int Nj, const int Nk);

connectionList *generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters);

void writeConnectivity(const char * filename, std::vector<std::pair<int,int>> *connect);

void writeLocation(const char * filename, float ** locationList, int ** throatCounters, int PNMax);

#endif /* defined(__networkgen__Generator__) */
