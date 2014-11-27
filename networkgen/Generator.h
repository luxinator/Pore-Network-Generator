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


location* generateLocation(const std::vector<std::pair<int, int>> * connectivity,const int Ni, const int Nj, const int N, const float Length);

std::vector<std::pair<int, int>> *generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array);

#endif /* defined(__networkgen__Generator__) */
