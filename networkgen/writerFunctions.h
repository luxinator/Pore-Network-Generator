
#ifndef writer_Functions_h
#define writer_Functions_h

#include "PoreNetwork.h"

void writeInlet_OutletPbs(const char * filename, PoreNetwork *pn);

void writeConnectivity(const char * filename, PoreNetwork *P_net);

void writeLocation(const char * filename, PoreNetwork *P_net);

void writeNetworkSpecs(const char * path, PoreNetwork *pn);

#endif