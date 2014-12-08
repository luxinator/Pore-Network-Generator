//
//  Eliminator.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__Eliminator__
#define __networkgen__Eliminator__

#include <iostream>
#include <vector>

#include "ArrayFunctions.h"
#include "PoreNetwork.h"

void EliminateThroats(PoreNetwork *P_net, float * C, int coordNr);

#endif /* defined(__networkgen__Eliminator__) */
