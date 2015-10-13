//
//  Eliminator.h
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

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
void eliminateThroats(PoreNetwork *P_net);

char * searchForIsolatedPB(PoreNetwork *P_net, size_t lengthTL);

char *searchForIsolatedPB_iterative(PoreNetwork *P_net, size_t lengthTL);

#endif /* defined(__networkgen__Eliminator__) */
