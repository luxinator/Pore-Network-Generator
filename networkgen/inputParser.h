//
//  inputParser.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 04-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__inputParser__
#define __networkgen__inputParser__
#include "PoreNetwork.h"

NetworkSpecs *readSpecsFile(const char *filename = "NetworkSpecs.in");

void loadNrs(const char *filename, PoreNetwork *P);

void loadPoreBodyLocations(const char *filename, PoreNetwork *P);

void loadThroats(const char *filename, PoreNetwork *P);

#endif /* defined(__networkgen__inputParser__) */
