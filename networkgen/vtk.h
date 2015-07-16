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

void writeVTK(std::string filename, const PoreNetwork *P_net, double * pb_values = nullptr, bool doCoords = false,
              const int precision = 0, int offset_start = 0, int offset_end = 0);

void writeVTK(std::string filename, const PoreNetwork *P_net, float * pb_values, bool doCoords = false,
              const int precision = 0, int offset_start = 0, int offset_end = 0);

void writeVTK(std::string filename, const PoreNetwork *P_net, bool * pb_values, bool doCroods = false,
              const int precision = 0, int offset_start = 0, int offset_end = 0);

void writeVTK(std::string filename, const PoreNetwork *P_net, char * pb_values, bool doCroods = false,
              const int precision = 0, int offset_start = 0, int offset_end = 0);

//void writeVTK(std::string filename, const PoreNetwork *P_net);

///void writeVTK(const char* filename, const PoreNetwork *P_net, const int precision = 0);

#endif /* defined(__networkgen__vtk__) */
