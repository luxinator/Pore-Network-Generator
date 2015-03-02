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

void loadNrs(const char *filename, PoreNetwork *P);

void loadPoreBodyLocations(const char *filename, PoreNetwork *P);

void loadThroats(const char *filename, PoreNetwork *P);

#endif /* defined(__networkgen__inputParser__) */
