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
#include <math.h>
#include "PoreNetwork.h"
#include "inputParser.h"
#include "ArrayFunctions.h"


/*
 * Cleans out a throatList in a PN, deleting all Flagged Entries
 * It does so by doing a member copy to an new array! NOT inplace!
 */

PoreNetwork::PoreNetwork(NetworkSpecs *ns){
    
    this->ns = ns;
    
    this->nrOfActivePBs = ns->Ni*ns->Nj*ns->Nk;
}

PoreNetwork::PoreNetwork(const char *networkSpecsFile){

    this->ns = readSpecsFile(networkSpecsFile);
    this->nrOfActivePBs = ns->Ni*ns->Nj*ns->Nk;
}

/*
 * Go through Throatslist and delete all entries with a throatvalue of Flag
 */
void PoreNetwork::removeFlaggedThroats(const int Flag){
    
    
    if(!this->throatList){
        return;
    }
    if(Flag == 0){
        std::cout << "Error in call to cleanThroatList \n\t Flag should be smaller then 0!";
        return;
    }
    
    int flagCounter = 0;
    size_t i = 0;
    
    while(this->throatList[0][i] != 0){
        if((this->throatList[0][i] == Flag || this->throatList[1][i] == Flag))
            flagCounter++;
        i++;
    }
    // gives index of 0 entry, which is also the length of the list
    
    size_t nrConnections = i;
    std::cout<< "Cleaning ThroatList ..." << std::endl;
    std::cout<< "  Amount of Connections: \t" << nrConnections << std::endl;
    std::cout<< "  Amount of Flagged pbs:    \t" << flagCounter   << std::endl;
    std::cout<< "  Max throats:          \t" << this->ns->Ni * this->ns->Nj * this->ns->Nk  * 13 << std::endl;
    
    
    // Since resizing an array in C++ is not done... we need to do a selective copy and delete the old array
    size_t newAmountOfConnections =  nrConnections - flagCounter + 1; // keep one extra for the [0,0] entry!
    
    int *t = new int[newAmountOfConnections * 2]; //temp
    int **newTL = new int*[2];
    newTL[0] = t;
    newTL[1] = t + newAmountOfConnections;
    
    // Copy all the connections
    size_t j = 0;
    for(i = 0; i < nrConnections; i++){
        if(this->throatList[1][i] != Flag && this->throatList[0][i] != Flag){
            newTL[0][j] = this->throatList[0][i];
            newTL[1][j] = this->throatList[1][i];
            j++;
        }
    }
    //put the guards in place
    newTL[0][newAmountOfConnections-1] = 0;
    newTL[1][newAmountOfConnections-1] = 0;

    //free the old throatList data
    delete [] this->throatList[0];
    
    // Repopulate the pointers
    this->throatList[0] = newTL[0];
    this->throatList[1] = newTL[1];
    
}

/*
 * Removes all porebodies who have a flag lower then minFlag
 * Renumbers and Updates the ThroatList, PoreBody-list and location-list
 */
void PoreNetwork::removeFlaggedPBs(char *pb_flag_list, int minFlag){
    
    std::cout<< "Removing Porebodies with a Flag value lower then: " << minFlag;
    size_t TL_Length = 0;
    
    // Look for gaurd, we now have the amount of throats in the network
    while(this->throatList[0][TL_Length] != 0){
        TL_Length++;
    }

    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    size_t i = 0;
    
    
    int *mappingList= new int[Ni*Nj*Nk]; // A list of how much a PBnumber should be lowerd -> mappingList[pn]
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
    for(i = 0; this->throatList[0][i] != 0; i ++){
        this->throatList[0][i] = this->throatList[0][i] - mappingList[this->throatList[0][i]];
        this->throatList[1][i] = this->throatList[1][i] - mappingList[this->throatList[1][i]];
    }
    
    // Clean the ThroatCounters and locations, delete all that are eleminated that's it, no re-numbering
    i = 1;
    for(size_t pn = 1; pn <= Ni*Nj*Nk; pn++){
        if(pb_flag_list[pn] >= minFlag){ //keep the value at pn
            this->throatCounter[0][i] = this->throatCounter[0][pn];
            this->throatCounter[1][i] = this->throatCounter[1][pn];
            
            this->locationList[0][i] = this->locationList[0][pn];
            this->locationList[1][i] = this->locationList[1][pn];
            this->locationList[2][i] = this->locationList[2][pn];
            i++;
        }
    }
    // Update the number of active porebodies in the network
    this->nrOfActivePBs = i;
    
    //flag the rest as garbage:
    // Future Note, change so that the arrays are deleted, we don't need to hog this memory
    for( i = i + 1; i <= Ni*Nj*Nk; i++){
        this->throatCounter[0][i] = -1;
        this->throatCounter[1][i] = -1;
        this->locationList[0][i]  = -1.0f;
        this->locationList[1][i]  = -1.0f;
        this->locationList[2][i]  = -1.0f;
    }
    
}



/*
 * return: connect[0][i] = source pb -> connect[1][i] = destination
 * Generate a half Connectivity map based upon the lattice distance between points.
 * The array return is huge and bulky.
 */

void PoreNetwork::generateConnectivity(){
    
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    
    
    if (!this->arr){
        std::cerr << "No pb Array supplied to generateConnectivity!" << std::endl;
        return;
    }
    int *** array = this->arr;
    
    // --- coords of pb under consideration
    int *coord = new int[3];
    int *coord_n = new int[3];
    
    // --- Init throatCounters arrays
    
    //Allocate a Part of Memory
    int *t = new int[2 * Ni*Nj*Nk + 2];
    this->throatCounter = new int*[2];
    //Point the two pointers to their places in the Large Part of memory
    this->throatCounter[0] = t;
    this->throatCounter[1] = t + Ni*Nj*Nk + 1;
    
    // Make a local variable
    int ** throatCounters = this->throatCounter;
    
    
    
    // --- Extend this to include variable connection amount based upon distance from Pore to neightbours!
    t = new int[2 * 13  *  Ni*Nj*Nk];// 13 connections per pb, we have Ni*Nj*Nk pbs and connection is a pair of 2 ints
    
    int **connection = new int*[2]; //[0] from [1] to
    connection[0] = t;
    connection[1] = t + (13 * Ni*Nj*Nk);
    
    for(size_t i = 0; i < 13  *  Ni*Nj*Nk; i ++){
        connection[0][i] = 0;
        connection[1][i] = 0;
    }
    // Add to this
    this->throatList = connection;
    
    std::cout << "number of PBs: "<<Ni*Nj*Nk << std::endl;
    
    for(size_t pn = 1; pn <= Ni*Nj*Nk; pn++){
        throatCounters[0][pn] = 0;
        throatCounters[1][pn] = 0;
    }
    
    int i = 0;
    double L = 0;
    float dist = 2.0f;
    
    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
        deflatten_3d(pn, Ni, Nj, Nk, coord); //coord from pb[pn]
        
        // boundary inlet only connect in flow dir
        if (coord[0] == 0){
            connection[0][i] = pn; //Pb nr
            connection[1][i] =  array[coord[0] + 1][coord[1]][coord[2]]; //connected to pb
            throatCounters[0][pn] += 1; //amount of connections of pb
            i++;//
            throatCounters[1][pn] += i;
            
            continue; // skip the rest, no more connections for this pb
        }
        // Connect to Boundary Outlet in x-dir and no more
        
        else if (coord[0] == Ni - 2){
            connection[0][i] = pn; //Pb nr
            connection[1][i] =  array[coord[0] + 1][coord[1]][coord[2]]; //connected to pb
            throatCounters[0][pn] += 1; //amount of connections of pb
            i++;
            throatCounters[1][pn] = i; // outlet has one throat in forward no more, no less!
            
            //continue; // skip the rest, no more connections for this pb
        }//else if
        
        // (pn+ Nj*Nk + (Nj*Nk /2 + 1) -> distance of one x-slice in x-dir
        //  Ni*Nj*Nk - Nj*Nk -> do not go further then second to last layer in x-dir
        //
        // This can be optimzed! within the same x-plane you have at best 3 throats per pb
        // for the next plane you have at best 9 connections.
        // the lowest pb in the nest x-plane has pn_n = pn + Nj*Nk - Nk - 1
        // the max pn_n = pn + Nj*Nk +Nk + 1
        // By using 2 for loops we lose the need to loop over half of the x-plane in front of pn
        //
        
        for( int pn_n = pn+1; pn_n < (pn+ Nj*Nk + (Nj*Nk /2 + 1)) &&
            pn_n <= Ni*Nj*Nk - Nj*Nk; pn_n++){
            
            deflatten_3d(pn_n, Ni, Nj, Nk, coord_n);
            
            L = sqrt(pow((double)(coord[0] - coord_n[0]), 2.0) +
                     pow((double)(coord[1] - coord_n[1]), 2.0)+
                     pow((double)(coord[2] - coord_n[2]), 2.0));
            
            if(L <= sqrt(dist)){
                connection[0][i] = pn; //Pb nr
                connection[1][i] = pn_n; //connected to pb
                throatCounters[0][pn] += 1; //amount of forward connections of pb
                
                i++;
                
            } // if
        } // for
        
        
        throatCounters[1][pn] += i; //nr of connection made in total
        
    }// for
    
    
    delete[] coord;
    delete[] coord_n;
    
    this->throatList = connection;
    this->throatCounter = throatCounters;
    
}


void PoreNetwork::generateLocation(){
    
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    
    
    
    float Length = this->ns->Length;
    
    //Allocate a large part of memory
    float *temp = new float[3 * Ni*Nj*Nk];
    // make a pointer to list of 3 pointers to floats
    float **locationList = new float*[3];
    
    // Point at exactly the right places in the Large Part of memory.
    locationList[0] = temp;
    locationList[1] = temp + (Ni*Nj*Nk);
    locationList[2] = temp + 2*(Ni*Nj*Nk);
    
    int *coord = new int[3];
    
    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
        deflatten_3d(pn, Ni, Nj, Nk, coord);
        
        locationList[0][pn] = coord[0] * Length;
        locationList[1][pn] = coord[1] * Length;
        locationList[2][pn] = coord[2] * Length;
        
    }
    this->locationList = locationList;
    
}

void PoreNetwork::generate_naive_array(){
    
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    
    int i,j,k;
    //  Allocate 3D Array, Yess Pointer->Pointer->Pointer->int ...
    // continous block needs some work, I do not understand it probably
    int ***porebodies = new int**[Ni];
    
    for(i = 0; i < Ni; i++){
        porebodies[i] = new int*[Nj];
        for(j = 0; j < Nj; j++){
            porebodies[i][j] = new int[Nk];
            for(k = 0; k < Nk; k++){
                porebodies[i][j][k] = i * Nj * Nk + j * Nk + k + 1;
            }
        }
    }
    this->arr = porebodies;

    
}


/*
 * Returns a sorted list of throats with connect[0][i] is source pb
 * Array is exactly as long as needed, will output length to *length
 */
void PoreNetwork::generateFullConnectivity(){
    
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    
    
    int **halfConnectivity = this->throatList;
    
    std::cout << "Generating Full Connectivity" << std::endl;
    size_t i = 0;
    size_t halfLength = 0;
    //maximum number of connections is:
    for(i = 0; i < Ni*Nj*Nk * 13; i++){
        if(halfConnectivity[0][i] == 0){
            halfLength = i;
            break;
        }
    }
    size_t maxConnections = halfLength  * 2 + 1; // Full connectivity is twich the size of half connectivity, and we need one extra place for the [0][0] guards
    
    //For Details see [generateConnectivity]
    int *t = new int[maxConnections * 2];
    
    int **connection = new int*[2];
    connection[0] = t;
    connection[1] = t + maxConnections;
    for(i = 0; i < maxConnections; i++){
        connection[0][i] = 0;
        connection[1][i] = 0;
    }
    
    for(i = 0; i < halfLength; i++){
        //copy
        connection[0][i * 2] = halfConnectivity[0][i];
        connection[1][i * 2] = halfConnectivity[1][i];
        
        
        //swap values and add as well...
        connection[0][i * 2 + 1] = halfConnectivity[1][i];
        connection[1][i * 2 + 1] = halfConnectivity[0][i];
        
    }
    
    bubbleSortList(connection, maxConnections-1); // Do NOT sort WITH the guards...
    
    
    this->throatList_full = connection;

}



/*
 * Delete Pore with PoreNumber i, a flag is placed in the throatlist and throatcounters
 */
size_t PoreNetwork::delelteThroat(size_t i, size_t deleted, int flag){
    
    this->throatList[1][i] = flag;
    deleted++;
    this->throatCounter[0][this->throatList[0][i]] -= 1;
    return deleted;
}

size_t PoreNetwork::delelteThroat(int i, int deleted, int flag){
    return this->delelteThroat(i, deleted, flag);
}

/*
 * Writes out the throatList until as entry with [0][0] is encounterd
 */
void writeConnectivity(const char * filename, PoreNetwork *pn){
    
    
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
    
    for(size_t i = 0;pn->throatList[0][i] != 0 ; i ++){
            file << pn->throatList[0][i]<< '\t' << pn->throatList[1][i] << std::endl;
    }
    
    file.close();
}


void writeLocation(const char * filename, PoreNetwork *P){
    
    
    
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
    for(size_t pn = 1; pn <= P->nrOfActivePBs; pn++){
        
        //file << '[' << pn << ']' << '\t';
        file << std::setw(8)<< P->locationList[0][pn]   << ' ';
        file << std::setw(8)<< P->locationList[1][pn]   << ' ';
        file << std::setw(8)<< P->locationList[2][pn]   << ' ';
        file << std::setw(8)<< P->throatCounter[0][pn] << ' ';
        file << std::setw(8)<< P->throatCounter[1][pn] << '\n';
        //std::cout<< throatCounters[0][pn] << '\t' <<  throatCounters[1][pn] << std::endl;
    }
    
    file.close();
}






