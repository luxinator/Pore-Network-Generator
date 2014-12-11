//
//  PoreNetwork.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef networkgen_PoreNetwork_h
#define networkgen_PoreNetwork_h



struct NetworkSpecs {
    int Ni, Nj, Nk;
    float Length;
    float Pi, Pj, Pk;
    float *C;
    
};

struct PoreNetwork{
    NetworkSpecs *ns;
    
    int ***arr; //contains the pb number at the lattic coordinates
    int **throatCounter; //number of prore throats per pb nr (1 based!) and number of throats preceding.
    int **throatList; // Connection map from pb nr -> pb nr. Is the half map!
    int nrOfConnections;
    float **locationList; // Location of a pb using its nr as index
    
};

/*
 * Cleans out a throatList in a PN, deleting all Flagged Entries
 * It does so by doing a member copy to an new array! NOT inplace!
 */
void cleanThroatList(PoreNetwork *pn, const int Flag = 0);


void writeConnectivity(const char * filename, int **connect);

void writeLocation(const char * filename, float ** locationList, int ** throatCounters, int PNMax);

void removeIsolatedPBs(PoreNetwork *pn, char *pb_flag_list, int minFlag);

#endif
