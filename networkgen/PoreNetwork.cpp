//
//  PoreNetwork.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include <iostream>
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
    if(!ns)
        return;
        
    
    this->nrOfActivePBs = ns->Ni*ns->Nj*ns->Nk;
}


/*
 * Go through Throatslist and delete all entries with a throatvalue of Flag
 * Usually done on the half connectivity map!
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
    
    for(i = 0; this->throatList[0][i] != 0; i++){
        if((this->throatList[0][i] == Flag || this->throatList[1][i] == Flag))
            flagCounter++;
    }
    // gives index of 0 entry, which is also the length of the list
    
    size_t nrConns = i;
    std::cout<< "Cleaning ThroatList ..." << std::endl;
    std::cout<< "  Amount of Connections: \t" << nrConns << std::endl;
    std::cout<< "  Amount of Flagged throats:    \t" << flagCounter   << std::endl;
    std::cout<< "  Max throats:          \t" << this->ns->Ni * this->ns->Nj * this->ns->Nk  * 13 << std::endl;
    
    
    // Since resizing an array in C++ is not possible... we need to do a selective copy and delete the old array
    size_t newAmountOfConnections =  nrConns - flagCounter + 1; // keep one extra for the [0,0] entry!
    
    int *t = new int[newAmountOfConnections * 2]; //temp
    int **newTL = new int*[2];
    newTL[0] = t;
    newTL[1] = t + newAmountOfConnections;
    
    // Copy all the connections
    size_t j = 0;
    
    for(i = 0; i < nrConns; i++){
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

    this->nrOfConnections = newAmountOfConnections-1;
    
}

/*
 * Removes all porebodies who have a flag lower then minFlag (REMINDER: minFlags is a CHAR!)
 * Done on the Full MAP!
 * renumbers the Throat counters and the location list
 */
void PoreNetwork::removeFlaggedPBs(char *pb_flag_list, char minFlag){
    
    if(!pb_flag_list){
        std::cerr << "PoreBodies flag list is empty, broken network?" << std::endl;
        return;
    }
    
    std::cout<< "Removing Porebodies with a Flag value lower then: " << (int)minFlag << std::endl;;
    
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    size_t i = 0;
    
    for(i = 0; this->throatList[0][i] != 0; i++){
        //Delete the connection FROM a flagged Pore
        if (pb_flag_list[this->throatList[0][i]] < minFlag) {
            this->throatList[0][i] = -1;
        }
        //Delete the connection TO a flagged pore
        if (pb_flag_list[this->throatList[1][i]] < minFlag) {
            this->throatList[0][i] = -1;
            this->throatList[1][i] = -1;
        }
    }
    
    // Do the Actually Deleting
    this->removeFlaggedThroats(-1);
    
    
    //pb_flag_list -> isolated pb's if minFlg
    //build a mask:
    int* mask = new int[Ni*Nj*Nk + 1];
    for(; i < Ni*Nj*Nk; i++){
        mask[i] = 0;
    }
    // Fill the Mask
    int cummulator = 0;
    for(i = 1; i <= Ni*Nj*Nk; i++){
        if(pb_flag_list[i] < minFlag){
            cummulator++;
        }
        mask[i] = cummulator;
        //std::cout << i << '\t' << mask[i] << '\t' << (int)pb_flag_list[i] << std::endl;
    }
    
    std::cout << "Amount PBs deleted: " << Ni*Nj*Nk - cummulator << std::endl;
    this->nrOfActivePBs = Ni*Nj*Nk - cummulator;
    
    
    //Use the mask to update the throatLists, the COMPLETE lists.
    for(i = 0; this->throatList[0][i] != 0; i++)
        this->throatList[0][i] = this->throatList[0][i] - mask[this->throatList[0][i]];
    
    for(i = 0; this->throatList[1][i] != 0; i++){
        this->throatList[1][i] = this->throatList[1][i] - mask[this->throatList[1][i]];
     
    }
    // Update the locations
    size_t newi;
    for(i = 1; i <= Ni*Nj*Nk; i ++){
        if(pb_flag_list[i] >= minFlag){ //Pore exists in new system
            newi = i - mask[i]; // get the new pbnr and update according
            this->locationList[0][newi] = this->locationList[0][i];
            this->locationList[1][newi] = this->locationList[1][i];
            this->locationList[2][newi] = this->locationList[2][i];
            
            this->throatCounter[0][newi] = this->throatCounter[0][i];
        }
    }
    //OverWrite the last part of the locationList with guards:
        for(i = this->nrOfActivePBs + 1; i < Ni*Nj*Nk; i++){
            this->locationList[0][i] = -1.0f;
            this->locationList[1][i] = -1.0f;
            this->locationList[2][i] = -1.0f;
        }
    
    
    int accumulator = 0;
    
    for(i = 1; i <= this->nrOfActivePBs; i ++){
        this->throatCounter[1][i] = accumulator + this->throatCounter[0][i];
        accumulator= this->throatCounter[1][i];
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
    
    
    for(size_t pn = 1; pn <= Ni*Nj*Nk; pn++){
        throatCounters[0][pn] = 0;
        throatCounters[1][pn] = 0;
    }
    
    
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
    
    int i = 0;
    double L = 0;
    double dist = this->ns->searchDistance;
    
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
            
            if(L <= dist){
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
    this->nrOfActivePBs = Ni*Nj*Nk;
    this->nrOfConnections = throatCounters[1][Ni*Nj*Nk];
    
}


void PoreNetwork::generateLocation(){
    
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    
    
    
    float Length = this->ns->pbDist;
    
    //Allocate a large part of memory
    float *temp = new float[3 * Ni*Nj*Nk + 3];
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
 * Array is exactly as long as needed, guard by an entry of [0,0]
 */
size_t PoreNetwork::generateFullConnectivity(){
    
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
    size_t maxConnections = halfLength  * 2 + 1; // Full connectivity is twice the size of half connectivity, and we need one extra place for the [0][0] guards
    
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

    std::cout << "\t Number of Connections Left: " << maxConnections - 1 << std::endl;

    
    return maxConnections;
}



/*
 * Delete Pore with PoreNumber i, a flag is placed in the throatlist, and throatcounter is diminished with 1
 * ThroatCounters[pn][1] is now invalid!
 */
size_t PoreNetwork::delelteThroat(size_t pn, size_t deleted, int flag){
    
    this->throatList[1][pn] = flag;
    deleted++;
    this->throatCounter[0][this->throatList[0][pn]] -= 1;
    this->throatCounter[1][this->throatList[0][pn]] = -1;
    return deleted;
}

size_t PoreNetwork::delelteThroat(int i, int deleted, int flag){
    return this->delelteThroat(i, deleted, flag);
}







