/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef writer_Functions_h
#define writer_Functions_h

#include "PoreNetwork.h"
#include "Combinator.h"

void writeConnectivity(const char * filename, PoreNetwork *P_net);

void writeLocation(const char * filename, PoreNetwork *P_net);

void writeNetworkSpecs(const char * path, PoreNetwork *pn);

void writeInterfacePores(const char * path, PoreNetwork *pn, Combinator *C);


#endif