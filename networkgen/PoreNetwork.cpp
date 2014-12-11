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
    
    // Copy
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
void removeIsolatedPBs(PoreNetwork *pn, char *pb_flag_list, int minFlag){
    
    int TL_Length = 0;
    
    // Look for gaurd, this is the amount of throats in the network
    while(pn->throatList[0][TL_Length] != 0){
        TL_Length++;
    }
    
    int Ni = pn->ns->Ni;
    int Nj = pn->ns->Nj;
    int Nk = pn->ns->Nk;
    
    int *mappingList= new int[Ni*Nj*Nk]; // A list of how much pn should be lowerd -> mappingList[pn]
    int cummulator = 0;
    for(int i = 1; i <= Ni*Nj*Nk; i++){
        if(pb_flag_list[i] < minFlag){
            cummulator += 1;
            
        }
        mappingList[i] = cummulator;
        std::cout<<'[' << i << "]\t" << mappingList[i] << std::endl;
    }
    
    // Change the Throatlist,
    for(int i = 0; pn->throatList[0][i] != 0; i ++)
    {
        pn->throatList[0][i] = pn->throatList[0][i] - mappingList[pn->throatList[0][i]];
        pn->throatList[1][i] = pn->throatList[1][i] - mappingList[pn->throatList[1][i]];
        
    }
    // Clean the ThroatCounters, delete all that are eleminated thats it not change to the data
    
    // Change the locationList, Same here
    
    
}

/*
 * Writes out the throatList until as entry with [0][0] is encounterd
 */
void writeConnectivity(const char * filename, int** connect){
    
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
    
    for(int i = 0;connect[0][i] != 0 ; i ++){
            file << connect[0][i]<< '\t' << connect[1][i] << std::endl;
    }
    
    file.close();
}


void writeLocation(const char * filename, float ** locationList, int ** throatCounters, int PNMax){
    
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
    for(int pn = 1; pn <= PNMax; pn++){
        
        //file << '[' << pn << ']' << '\t';
        file << std::setw(8)<< locationList[0][pn]   << ' ';
        file << std::setw(8)<< locationList[1][pn]   << ' ';
        file << std::setw(8)<< locationList[2][pn]   << ' ';
        file << std::setw(8)<< throatCounters[0][pn] << ' ';
        file << std::setw(8)<< throatCounters[1][pn] << '\n';
        //std::cout<< throatCounters[0][pn] << '\t' <<  throatCounters[1][pn] << std::endl;
    }
    
    file.close();
}