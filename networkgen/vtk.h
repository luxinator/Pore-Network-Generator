//
//  vtk.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 02-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef __networkgen__vtk__
#define __networkgen__vtk__

#include <stdio.h>
#include "Generator.h"

void writeVTK(const char* filename, connectionList* connect , float** locationList, int Ni, int Nj, int Nk);

#endif /* defined(__networkgen__vtk__) */
