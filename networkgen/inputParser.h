//
//  inputParser.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 04-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__inputParser__
#define __networkgen__inputParser__

#include <stdio.h>


struct NetworkSpecs {
    int Ni, Nj, Nk;
    float Length;
    
};

NetworkSpecs *readSpecsFile(const char *filename = "NetworkSpecs.in");

#endif /* defined(__networkgen__inputParser__) */
