//
//  Generator.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__Generator__
#define __networkgen__Generator__

#include "PoreNetwork.h"


float ** generateLocation(const int Ni, const int Nj, const int Nk, PoreNetwork *P_net);

int ** generateConnectivity(const int Ni, const int Nj, const int Nk, PoreNetwork *P_net);

int **generateFullConnectivity(const int Ni, const int Nj, const int Nk, PoreNetwork *P_net);

#endif /* defined(__networkgen__Generator__) */
