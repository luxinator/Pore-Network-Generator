//
//  inputParser.h
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __networkgen__inputParser__
#define __networkgen__inputParser__
#include "PoreNetwork.h"

NetworkSpecs *readSpecsFile(const char *filename = "NetworkSpecs.in");

int loadNrs(const char *filename, PoreNetwork *P);

int loadPoreBodyLocations(const char *filename, PoreNetwork *P);

int loadThroats(const char *filename, PoreNetwork *P);

#endif /* defined(__networkgen__inputParser__) */
