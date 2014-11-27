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


// Just a Wrapper for a tuple containing x,y,z, nr of throats, nr of total throats incl.
typedef std::tuple<float, float, float, int, int> location;


void generateLocation(const float Length, int **throatCounters, const int Ni, const int Nj, const int Nk);

std::vector<std::pair<int, int>> *generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters);

#endif /* defined(__networkgen__Generator__) */
