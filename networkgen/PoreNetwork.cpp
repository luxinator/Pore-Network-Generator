//
//  PoreNetwork.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include "PoreNetwork.h"

/*
 * Cleans out a throatList in a PN, deleting all Flagged Entries
 * It does so by doing a member copy to an new array! NOT inplace!
 */

void cleanThroatList(PoreNetwork *pn, const int Flag){
    
    
    if(!pn->throatList){
        return;
    }
    
    int flagCounter = 0;
    int i = 0;
    
    while(pn->throatList[0][i] != 0){
        if(Flag != 0 && (pn->throatList[0][i] == Flag || pn->throatList[1][i] == Flag))
            flagCounter++;
        i++;
    }
    // gives index of 0 entry, which is also the length of the list
    
    int nrConnections = i;
    std::cout<< "Cleaning ThroatList ..." << std::endl;
    std::cout<< "  Amount of Connection: \t" << nrConnections << std::endl;
    std::cout<< "  Amount of Flagged:    \t" << flagCounter   << std::endl;
    std::cout<< "  Max throats:          \t" << pn->ns->Ni * pn->ns->Nj * pn->ns->Nk  * 13 << std::endl;
    
    
    // Since resizing an array in C++ is not done... we need to do a selective copy and delete the old array
    int newAmountOfConnections =  nrConnections - flagCounter + 1; // keep one extra for the [0,0] entry!
    int *t = new int[newAmountOfConnections * 2];
    int **newTL = new int*[2];
    newTL[0] = t;
    newTL[1] = t + newAmountOfConnections;
    
    // Copy all the connections
    int j = 0;
    for(i = 0; i < nrConnections; i++){
        if(pn->throatList[1][i] != Flag && pn->throatList[0][i] != Flag){
            newTL[0][j] = pn->throatList[0][i];
            newTL[1][j] = pn->throatList[1][i];
            j++;
        }
    }
    //guards
    newTL[0][newAmountOfConnections-1] = 0;
    newTL[1][newAmountOfConnections-1] = 0;

    //free the old throatList data
    delete [] pn->throatList[0];
    
    // Repopulate the pointers
    pn->throatList[0] = newTL[0];
    pn->throatList[1] = newTL[1];
    
}

/*
 * Removes all porebodies who have a flag lower then minFlag
 * Renumbers and Updates the ThroatList and PB-list and location
 */
void removeIsolatedPBs(PoreNetwork *P_net, char *pb_flag_list, int minFlag){
    
    int TL_Length = 0;
    
    // Look for gaurd, this is the amount of throats in the network
    while(P_net->throatList[0][TL_Length] != 0){
        TL_Length++;
    }
    
    int Ni = P_net->ns->Ni;
    int Nj = P_net->ns->Nj;
    int Nk = P_net->ns->Nk;
    int i = 0;
    
    
    int *mappingList= new int[Ni*Nj*Nk]; // A list of how much pn should be lowerd -> mappingList[pn]
    int cummulator = 0;
    for(int i = 1; i <= Ni*Nj*Nk; i++){
        if(pb_flag_list[i] < minFlag){
            cummulator += 1;
            
        }
        mappingList[i] = cummulator;
        //std::cout<<'[' << i << "]\t" << mappingList[i] << std::endl;
    }
    std::cout<< "\t PoreBodies set to Inactive: "<< cummulator << std::endl;
    // Change the Throatlist, according to the deletion of pbs.
    for(i = 0; P_net->throatList[0][i] != 0; i ++)
    {
        P_net->throatList[0][i] = P_net->throatList[0][i] - mappingList[P_net->throatList[0][i]];
        P_net->throatList[1][i] = P_net->throatList[1][i] - mappingList[P_net->throatList[1][i]];
        
    }
    
    
    
    // Clean the ThroatCounters and locations, delete all that are eleminated that's it, no re-numbering
    i = 1;
    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
        if(pb_flag_list[pn] >= minFlag){ //keep the value at pn
            P_net->throatCounter[0][i] = P_net->throatCounter[0][pn];
            P_net->throatCounter[1][i] = P_net->throatCounter[1][pn];
            
            P_net->locationList[0][i] = P_net->locationList[0][pn];
            P_net->locationList[1][i] = P_net->locationList[1][pn];
            P_net->locationList[2][i] = P_net->locationList[2][pn];
            i++;
        }
    }
    P_net->nrOfActivePBs = i-1;
    //flag the rest as garbage:
    for( ; i <= Ni*Nj*Nk; i++){
        P_net->throatCounter[0][i] = -1;
        P_net->throatCounter[1][i] = -1;
        P_net->locationList[0][i]  = -1.0f;
        P_net->locationList[1][i]  = -1.0f;
        P_net->locationList[2][i]  = -1.0f;
    }
    
}

/*
 * Writes out the throatList until as entry with [0][0] is encounterd
 */
void writeConnectivity(const char * filename, PoreNetwork *P_net){
    
    
    std::ofstream file;
    if(!filename){
        std::cerr << "No filename specified! " << std::endl;
    }
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    for(int i = 0;P_net->throatList[0][i] != 0 ; i ++){
            file << P_net->throatList[0][i]<< '\t' << P_net->throatList[1][i] << std::endl;
    }
    
    file.close();
}


void writeLocation(const char * filename, PoreNetwork *P_net){
    
    
    
    std::ofstream file;
    if( filename == nullptr){
        std::cerr << "No filename specified! " << std::endl;
    }
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    // set output type to scientific
    file.setf(std::ios_base::scientific);
    for(int pn = 1; pn <= P_net->nrOfActivePBs; pn++){
        
        //file << '[' << pn << ']' << '\t';
        file << std::setw(8)<< P_net->locationList[0][pn]   << ' ';
        file << std::setw(8)<< P_net->locationList[1][pn]   << ' ';
        file << std::setw(8)<< P_net->locationList[2][pn]   << ' ';
        file << std::setw(8)<< P_net->throatCounter[0][pn] << ' ';
        file << std::setw(8)<< P_net->throatCounter[1][pn] << '\n';
        //std::cout<< throatCounters[0][pn] << '\t' <<  throatCounters[1][pn] << std::endl;
    }
    
    file.close();
}