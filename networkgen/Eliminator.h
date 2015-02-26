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

/*
 * Eliminate throats based upthen there chance to survive
 *
 * C[0] = x-dir; C[1] = y-dir; C[2] = z-dir
 * C[3] = x - -y; C[4] = x - +y;
 * C[5] = x - -z; C[6] = x - +z;
 * C[7] = y - -z; C[8] = y - +z;
 * C[9] = -y - -z;C[10]= -y - + z;
 *
 * Max coordination number is 6 at the moment (in forward x dir!)
 *
 */
void eliminateThroats(PoreNetwork *P_net, int coordNr);

char * searchForIsolatedPB(PoreNetwork *P_net, size_t lengthTL);

#endif /* defined(__networkgen__Eliminator__) */
