//
//  vtk.h
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __networkgen__vtk__
#define __networkgen__vtk__

#include "PoreNetwork.h"

//void writeVTK(const char* filename, PoreNetwork *P_net, float * pb_flags, const int precision = 0);

void writeVTK(const char* filename, const PoreNetwork *P_net, const int precision = 0);

#endif /* defined(__networkgen__vtk__) */
