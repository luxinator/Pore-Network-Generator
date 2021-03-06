//
//  PoreNetwork.cpp
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <math.h>
#include <fstream>
#include <random>
#include <string.h>
#include "PoreNetwork.h"
#include "inputParser.h"
#include "ArrayFunctions.h"


/*
 * Cleans out a throatList in a PN, deleting all Flagged Entries
 * It does so by doing a member copy to an new array! NOT inplace!
 */


int PoreNetwork::checkInput(){

    // if (this->ns->Ni < 3 && this->ns->flowDirs[0]){
    //     std::cerr << "ERROR: The X-boundary Condition is turnend on\nBut Ni is smaller then 3: Not Enough PB's for conditions!\n" << std::endl;
    //     return 1;
    // }

    // if (this->ns->Nj < 3 && this->ns->flowDirs[1]){
    //     std::cerr << "ERROR: The Y-boundary Condition is turnend on\nBut Nj is smaller then 3: Not Enough PB's for conditions!\n" << std::endl;
    //     return 1;
    // }

    // if (this->ns->Nk < 3 && this->ns->flowDirs[2]){
    //     std::cerr << "ERROR: The Z-boundary Condition is turnend on\nBut Nk is smaller then 3: Not Enough PB's for conditions!\n" << std::endl;
    //     return 1;
    // }

    this->nrOfActivePBs = ns->Ni*ns->Nj*ns->Nk;
    this->nrOfInlets = 0;
    this->nrOfOutlets = 0;

    return 0;
}

PoreNetwork::PoreNetwork(NetworkSpecs *ns) : PoreNetwork(){


    this->ns = ns;

    if(checkInput() != 0)
        ns = nullptr;

}

PoreNetwork::PoreNetwork(const char *networkSpecsFile) : PoreNetwork(){


    this->ns = readSpecsFile(networkSpecsFile);
    if(!ns)
        return;

    if(checkInput() != 0)
        ns = nullptr;

}

/*
 * Empty Constructor, all pointers of the arrays and lists are 0x0
 */

PoreNetwork::PoreNetwork(){

	// Zero out all the memory, all pointers to 0x0 and all values to 0;
	this->arr 				= nullptr;
	this->pb_sizeList		= nullptr;
	this->locationList 		= nullptr;
	this->ns				= nullptr;
	this->periodicThroats	= nullptr;
	this->throatCounter		= nullptr;
	this->throatList		= nullptr;
	this->throatList_full	= nullptr;
	this->nrOfActivePBs 	= 0;
	this->nrOfConnections 	= 0;
	this->nrOfInlets 		= 0;
	this->nrOfOutlets		= 0;
	this->periodicListLength= 0;
}

PoreNetwork::PoreNetwork(const PoreNetwork& other, std::string newName) : PoreNetwork(){

    this->ns = new NetworkSpecs(*other.ns);
    this->ns->name = newName;
    this->throatList_full =  nullptr;

    std::cout << "Copying PoreNetwork: " << other.ns->name << " To: " << this->ns->name << std::endl;

    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;

    for(size_t i = 0; i < 26; i ++)
        this->ns->C[i] = other.ns->C[i];
    this->ns->searchDistance = other.ns->searchDistance;
    this->ns->pbDist         = other.ns->pbDist;
    this->ns->periodicBounndaries = other.ns->periodicBounndaries;
    this->ns->constantPBSize = other.ns->constantPBSize;
    for(size_t i = 0; i < 3; i++)
        this->ns->flowDirs[i] = other.ns->flowDirs[i];


    if(other.arr){
        this->arr = new int**[this->ns->Ni];

        for(size_t i = 0; i < this->ns->Ni; i++){
            this->arr[i] = new int*[this->ns->Nj];
            for(size_t j = 0; j < this->ns->Nj; j++){
                this->arr[i][j] = new int[this->ns->Nk];
                for(size_t k = 0; k < this->ns->Nk; k++){
                    this->arr[i][j][k] = other.arr[i][j][k];
                }
            }
        }
    } else
        this->arr = nullptr;

    //Allocate a large part of memory
    float *temp = new float[3 * Ni*Nj*Nk + 3];
    // make a pointer to list of 3 pointers to floats
    this->locationList = new float*[3];

    // Point at exactly the right places in the Large Part of memory.
    this->locationList[0] = temp;
    this->locationList[1] = temp + (Ni*Nj*Nk);
    this->locationList[2] = temp + 2*(Ni*Nj*Nk);

    for(size_t pn = 1; pn <= Ni*Nj*Nk; pn++){
        this->locationList[0][pn] = other.locationList[0][pn];
        this->locationList[1][pn] = other.locationList[1][pn];
        this->locationList[2][pn] = other.locationList[2][pn];
    }

    // -- throatList
    size_t i = 0;

    int *t = new int[other.nrOfConnections * 2];
    this->throatList = new int*[2];
    this->throatList[0] = t;
    this->throatList[1] = t + other.nrOfConnections;

    for ( i = 0; i < other.nrOfConnections; i++) {
        this->throatList[0][i] = other.throatList[0][i];
        this->throatList[1][i] = other.throatList[1][i];
    }

    // -- throatCounters
    t = new int[2 * other.nrOfActivePBs + 2];
    this->throatCounter = new int*[2];

    this->throatCounter[0] = t;
    this->throatCounter[1] = t + other.nrOfActivePBs + 1; // + 1 for the 0 guard

    size_t pn;
    for(pn = 1; pn <= other.nrOfActivePBs; pn++){
        this->throatCounter[0][pn] = other.throatCounter[0][pn];
        this->throatCounter[1][pn] = other.throatCounter[1][pn];
    }
	
	this->pb_sizeList = new float[other.nrOfActivePBs + 1];
	for(pn = 1; pn <= other.nrOfActivePBs; pn++){
        this->pb_sizeList[pn] = other.pb_sizeList[pn];
    }
	
    // -- periodicThroats
    this->periodicThroats = new int[other.periodicListLength];
    for (i = 0; i < other.periodicListLength; i ++){
        this->periodicThroats[i] = other.periodicThroats[i];
        //std::cout << i << "--" << other.periodicThroats[i]<< std::endl;
    }

    // -- Add active PBS and Active PTs!
    this->nrOfActivePBs = other.nrOfActivePBs;
    this->nrOfConnections = other.nrOfConnections;
    this->periodicListLength = other.periodicListLength;

    // Guard it against unvaild access!
    this->throatList_full =  nullptr;

	
    std::cout << "Done Copying" << std::endl;

}


PoreNetwork::~PoreNetwork(){

    if (arr) {
        for (size_t i = 0; i < this->ns->Ni; i++)
            for (size_t j = 0; j < this->ns->Nj; j++) {
                delete [] arr[i][j];
            }
        delete [] arr;
        arr = nullptr;
    }
    if (throatCounter) {
        delete [] throatCounter[0];
        throatCounter[0] = nullptr;
        delete [] throatCounter;
        throatCounter = nullptr;
    }
    if (throatList) {
        delete [] throatList[0];
        delete [] throatList;
        throatCounter = nullptr;
    }
	if (throatList_full) {
        delete [] throatList_full[0];
        delete [] throatList_full;
        throatList_full = nullptr;
    }
    if (locationList) {
        delete [] locationList[0];
        locationList[0] = nullptr;
        delete [] locationList;
        locationList = nullptr;
    }
    if(periodicThroats)
        delete[] periodicThroats;
		
	if(ns)
		delete ns;
	
}


PoreNetwork::PoreNetwork(const std::string networkSpecs_file) : PoreNetwork(){

    this->ns = readSpecsFile(networkSpecs_file.c_str());


    std::string path = networkSpecs_file.substr(0, (networkSpecs_file.length() - 15));
    std::string name = this->ns->name;

    int Ni = this->ns->Ni;
    int Nj = this->ns->Nj;
    int Nk = this->ns->Nk;

    loadNrs((path + name + "_specs.txt").c_str(), this);

    float *temp = new float[3 * this->nrOfActivePBs + 3];
    this->locationList = new float*[3];

    this->locationList[0] = temp;
    this->locationList[1] = temp + this->nrOfActivePBs;
    this->locationList[2] = temp + 2*this->nrOfActivePBs;

    this->pb_sizeList = new float[nrOfActivePBs + 1];

    int *t = new int[2 * this->nrOfActivePBs + 2];
    this->throatCounter = new int*[2];
    this->throatCounter[0] = t;
    this->throatCounter[1] = t + this->nrOfActivePBs + 1;

    loadPoreBodyLocations((path + name + "_loc.txt").c_str(), this);

    t = new int[this->nrOfConnections * 2];
    this->throatList = new int*[2];
    this->throatList[0] = t;
    this->throatList[1] = t + this->nrOfConnections;

    this->periodicListLength = (int)(Ni*Nj + Nj*Nk + Nk*Ni);

    this->periodicThroats = new int[periodicListLength];
    for (size_t i = 0; i < periodicListLength; i++)
        this->periodicThroats[i] = 0;

	t = nullptr;
    loadThroats((path + name + "_conn.txt").c_str(), this);
}

/*
 * Go through Throatslist and delete all entries with a throatvalue of Flag
 * Usually done on the half connectivity map, no implentation for full is available atm
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

    for(i = 0; i < this->nrOfConnections; i++){
        if((this->throatList[0][i] == Flag || this->throatList[1][i] == Flag))
            flagCounter++;
    }
    // gives index of 0 entry, which is also the length of the list

    size_t nrConns = i;
    std::cout<< "Cleaning ThroatList ..." << std::endl;
    std::cout<< "\tAmount of Connections: \t" << nrConns << std::endl;
    std::cout<< "\tAmount of Flagged throats:    \t" << flagCounter   << std::endl;
    //std::cout<< "  Max throats:          \t" << this->ns->Ni * this->ns->Nj * this->ns->Nk  * 13 << std::endl;


    // Since resizing an array in C++ is not possible... we need to do a selective copy and delete the old array
    size_t newAmountOfConnections =  nrConns - flagCounter + 1; // keep one extra for the [0,0] guard!

    int *t = new int[newAmountOfConnections * 2]; //temp
    int **newTL = new int*[2];
    newTL[0] = t;
    newTL[1] = t + newAmountOfConnections;

    // Copy all the connections
    size_t j = 0;
    size_t Pc = 0;

    for(i = 0; i < this->periodicListLength; i++){
        this->periodicThroats[i] = 0;
    }

    for(i = 0; i < nrConns; i++){
        if(this->throatList[1][i] != Flag && this->throatList[0][i] != Flag){
            newTL[0][j] = this->throatList[0][i];
            newTL[1][j] = this->throatList[1][i];

            if(newTL[0][j] > newTL[1][j]){
                this->periodicThroats[Pc] = (int)j;
                Pc++;
            }
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

int PoreNetwork::rebuildThroatCounters(){

    std::cout << "Rebuilding ThroatCounters" << std::endl;
    size_t accumulator = 0;
  
    //Clean the throatcounters
    for(size_t i = 0; i <= this->nrOfActivePBs;i++){
	this->throatCounter[0][i] = 0;
	this->throatCounter[1][i] = 0;
    }
    
    for(size_t i = 0; i < this->nrOfConnections;i++){
      int pn = this->throatList[0][i];
      this->throatCounter[0][pn] += 1;
    }
    
    for(size_t i = 1; i <= this->nrOfActivePBs; i++){
	this->throatCounter[1][i] = this->throatCounter[1][i-1] + this->throatCounter[0][i];
    }
    
    accumulator = this->throatCounter[1][this->nrOfActivePBs];
    
    if(accumulator != this->nrOfConnections) {
        std::cerr << "accumulator : nr of Throats!" << std::endl;
        std::cerr << accumulator << '\t' << this->nrOfConnections << '\t' << this->nrOfConnections - accumulator <<  std::endl;
    }

    return 0;

}

/*
 * Removes all porebodies who have a flag lower then minFlag (REMINDER: minFlags is a CHAR!)
 * renumbers the Throat counters and the location list as well, only the half map is renumberd!
 * For a fully connected and renumberd map simply rerun pn->generateFullConnectivity()
 */
int PoreNetwork::removeFlaggedPBs(char *pb_flag_list, char minFlag){

    if(!pb_flag_list){
        std::cerr << "PoreBodies flag list is empty, broken network?" << std::endl;
        return -1;
    }

    std::cout<< "Removing Porebodies with a Flag value lower then: " << (int)minFlag << std::endl;;

    size_t i = 0;

	size_t in = 0;
	size_t out = 0;
	
    for(i = 0; i < this->nrOfConnections; i++){
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
	
	//Check for Inlet or Outlet changes
	for(i = 0; i < this->nrOfInlets; i++){
		if(this->throatList[0][i] == -1 )
			in++;
	}
	for(i = nrOfConnections - nrOfOutlets; i < this->nrOfConnections; i++)
		if(this->throatList[0][i] == -1)
			out++;
    
	//std::cout << nrOfInlets - in << '\t' << nrOfOutlets - out << std::endl;
	nrOfInlets -= in; nrOfOutlets-= out;
	
    // Do the Actually Deleting of the Throats
    this->removeFlaggedThroats(-1);
	
	//std::cout << nrOfInlets << '\t' << nrOfOutlets << std::endl;
	

    //pb_flag_list -> isolated pb's if minFlg
    //build a mask:
    int* mask = new int[this->nrOfActivePBs + 1];
    for(i = 0; i <= this->nrOfActivePBs; i++){
        mask[i] = 0;
    }
    // Fill the Mask
    int cummulator = 0;
    for(i = 1; i <= this->nrOfActivePBs; i++){
        if(pb_flag_list[i] < minFlag){
            cummulator++;
        }
        mask[i] = cummulator;
        //std::cout << i << '\t' << mask[i] << '\t' << (int)pb_flag_list[i] << std::endl;
    }

    std::cout << "Amount PBs deleted: " << cummulator << std::endl;
    size_t oldNrOfActivePBs = this->nrOfActivePBs;
    this->nrOfActivePBs = this->nrOfActivePBs - cummulator;


    // --- Use the mask to update the throatLists, the COMPLETE lists.
    for(i = 0; i < this->nrOfConnections; i++)
        this->throatList[0][i] = this->throatList[0][i] - mask[this->throatList[0][i]];

    for(i = 0; i < this->nrOfConnections; i++){
        this->throatList[1][i] = this->throatList[1][i] - mask[this->throatList[1][i]];
    }

    //And if there there are periodic throats, update them as well. Periodic throats have the cool property: throatlist[0][i] > throatlist[1][i]
    if(this->ns->periodicBounndaries) {
        size_t j = 0;
        for(i = 0; i < this->nrOfConnections; i++){
            if(this->throatList[0][i] > this->throatList[1][i]){
                this->periodicThroats[j] = (int)i;
                j++;
            }
        }

        // The rest is garbage so delete the entry
        for( ; j < 	this->periodicListLength; j++)
            this->periodicThroats[j] = 0;
    }

    // --- Update the locations
    size_t newi = oldNrOfActivePBs;

    for(i = 1; i <= oldNrOfActivePBs; i ++){
        if(pb_flag_list[i] >= minFlag){ //Pore exists in new system
            newi = i - mask[i]; // get the new pbnr and update according
            this->locationList[0][newi] = this->locationList[0][i];
            this->locationList[1][newi] = this->locationList[1][i];
            this->locationList[2][newi] = this->locationList[2][i];

            this->throatCounter[0][newi] = this->throatCounter[0][i];
			this->pb_sizeList[newi] = this->pb_sizeList[i];
        }
    }
    //OverWrite the last part of the locationList with guards:
    for(i = newi + 1; i < oldNrOfActivePBs; i++){
        this->locationList[0][i] = -1.0f;
        this->locationList[1][i] = -1.0f;
        this->locationList[2][i] = -1.0f;
		this->pb_sizeList[i] = 0.0f;
    }

    delete[] pb_flag_list;
    delete[] mask;
	
	return cummulator;
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
    //int *** array = this->arr;

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
    //Set all places to 0
    for(size_t pn = 1; pn <= Ni*Nj*Nk; pn++){
        this->throatCounter[0][pn] = 0;
        this->throatCounter[1][pn] = 0;
    }

    // 12 + periodic, we have Ni*Nj*Nk pbs and connection is a pair of 2 ints
    size_t maxConnections = 12 * Ni*Nj*Nk;
    if (this->ns->periodicBounndaries) {
        maxConnections += Ni*Nj + Nj*Nk + Ni*Nk;
    }

    t = new int[2 * maxConnections];

    int **connection = new int*[2]; //from [0] to [1]
    connection[0] = t;
    connection[1] = t + maxConnections;

    for(size_t i = 0; i < maxConnections; i ++){
        connection[0][i] = 0;
        connection[1][i] = 0;
    }
    this->throatList = connection;

    std::cout << "number of PBs: "<<Ni*Nj*Nk << std::endl;


    // PeriodicBoundaries are Places in ThroatList containing Periodic throats
    if(this->ns->periodicBounndaries){
        this->periodicListLength = Ni*Nj + Nj*Nk + Nk*Ni;
        this->periodicThroats = new int[Ni*Nj + Nj*Nk + Ni*Nk + 1];
        for(size_t i = 0; i < Ni*Nj + Nj*Nk + Ni*Nk + 1; i++){
            this->periodicThroats[i] = 0;
        }
    }
    size_t periodicTrs = 0;


    int i = 0;
    double L = 0;
    double dist = this->ns->searchDistance;

    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
        deflatten_3d(pn, Ni, Nj, Nk, coord); //coord from pb[pn]


        // ---------- Generates the inner pore porebody throats -------- \\
        // (pn+ Nj*Nk + (Nj*Nk /2 + 1) -> distance of one x-slice in x-dir
        //  Ni*Nj*Nk - Nj*Nk -> do not go further then second to last layer in x-dir
        for( int pn_n = pn+1; pn_n < (pn+ Nj*Nk + (Nj*Nk /2 + 1)) &&
            pn_n <= Ni*Nj*Nk; pn_n++){

            deflatten_3d(pn_n, Ni, Nj, Nk, coord_n);

            L = sqrt(pow((double)(coord[0] - coord_n[0]), 2.0) +
                     pow((double)(coord[1] - coord_n[1]), 2.0) +
                     pow((double)(coord[2] - coord_n[2]), 2.0));

            if(L <= dist){
                connection[0][i] = pn; //Pb nr
                connection[1][i] = pn_n; //connected to pb
                this->throatCounter[0][pn] += 1; //amount of forward connections of pb

                i++;

            } // if

        } // for

        //--- Add Periodic connection, Inner network has all boundaries as periodic
        if(this->ns->periodicBounndaries && coord[0] == Ni - 1 && Ni > 1){
            connection[0][i] = pn;
            connection[1][i] = this->arr[0][coord[1]][coord[2]];
            this->throatCounter[0][pn] += 1;
            this->periodicThroats[periodicTrs] = i;
            //std::cout << this->periodicThroats[periodicTrs] << std::endl;

            periodicTrs++;
            i++;
         }
        if(this->ns->periodicBounndaries && coord[1] == Nj - 1 && Nj > 1){
            connection[0][i] = pn;
            connection[1][i] = this->arr[coord[0]][0][coord[2]];//pn - ((Nk - 1) * Nj);
            this->throatCounter[0][pn] += 1;
            this->periodicThroats[periodicTrs] = i;
            //std::cout << this->periodicThroats[periodicTrs] << std::endl;

            periodicTrs++;
            i++;
        }
        if(this->ns->periodicBounndaries && coord[2] == Nk - 1 && Nk > 1){
            connection[0][i] = pn;
            connection[1][i] = this->arr[coord[0]][coord[1]][0];//pn - (Nj - 1);
            this->throatCounter[0][pn] += 1;
            this->periodicThroats[periodicTrs] = i;
           // std::cout << this->periodicThroats[periodicTrs] << std::endl;

            periodicTrs++;
            i++;
        }

        this->throatCounter[1][pn] += i; //nr of connection made in total

    }// for

    delete[] coord;
    delete[] coord_n;

    this->throatList = connection;
    this->nrOfActivePBs = Ni*Nj*Nk;
    //std::cout << "stuff: "<<i << '\t' << Ni*Nj*Nk << '\t' << this->throatCounter[1][Ni*Nj*Nk] << std::endl;
    this->nrOfConnections = this->throatCounter[1][Ni*Nj*Nk];
    this->periodicListLength = periodicTrs;

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

        locationList[0][pn] = coord[0] * Length; // + Length, because boundary pbs are on (0,0,0)
        locationList[1][pn] = coord[1] * Length;
        locationList[2][pn] = coord[2] * Length;

    }

    delete[] coord;
    this->locationList = locationList;

}

void PoreNetwork::generate_naive_array(){

    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;

    size_t i,j,k;
    //  Allocate 3D Array, Yess Pointer->Pointer->Pointer->int ...
    // continous block needs some work, I do not understand it probably
    int ***porebodies = new int**[Ni];

    for(i = 0; i < Ni; i++){
        porebodies[i] = new int*[Nj];
        for(j = 0; j < Nj; j++){
            porebodies[i][j] = new int[Nk];
            for(k = 0; k < Nk; k++){
                porebodies[i][j][k] = (int)i * Nj * Nk + (int)j * Nk + (int)k + 1;
            }
        }
    }
    this->arr = porebodies;


}


/*
 * Creates a list padded with [amount] -1
 * If (headPadding) -1 will be at the head of the list
 * if (!headPadding) -1 will at the end of the list
 */

template <typename T> T** PoreNetwork::paddedList(size_t amount, T **List, size_t nrOfCols ,size_t currentSize, bool headPadding) {
    size_t i,j;

    T *tList = new T[(currentSize + amount) * nrOfCols];
    T **newList = new T*[nrOfCols];

    for(j = 0; j < nrOfCols; j++)
        newList[j] = tList + (currentSize + amount) * j;

    for(i = 0; i < currentSize + amount; i++)
        for(j = 0; j < nrOfCols; j++)
            newList[j][i] = (T)-1;

    if(headPadding) {
        for(i = amount; i < currentSize + amount ; i++)
            for(j = 0; j < nrOfCols; j++)
                newList[j][i] = List[j][i - amount];

    } else {
        for(i = 0; i < currentSize ; i++)
            for(j = 0; j < nrOfCols; j++)
                newList[j][i] = List[j][i];
    }
    return newList;
}

template <typename T> T* PoreNetwork::paddedList(size_t amount, T *List, size_t currentSize, bool headPadding) {
    size_t i;
	
    T *newList = new T[(currentSize + amount)];
    
    for(i = 0; i < currentSize + amount; i++)
        newList[i] = (T)-1;

    if(headPadding) {
        for(i = amount; i < currentSize + amount ; i++)
            newList[i] = List[i - amount];

    } else {
        for(i = 0; i < currentSize ; i++)
			newList[i] = List[i];
    }
    return newList;
}

/*
 * Generates the inlet and outlet pores around the inner network.
 * This is a Long and Ugly Method, but its kind of magical. Every
 * If statement is slightly different from the others, this is the easiest
 * way to do this...
 *
 *
 *  !!!! This Code needs some revision! but not  now...
 */

void PoreNetwork::generateBoundary(size_t dir, float inletSize, float outletSize, float inletThroatLength, float outletThroatLength) {

    if (dir > 2) {
        std::cerr << "\nCRITICAL ERROR: COULD NOT GENERATE BOUNDARIES!" << std::endl;
        return;
    }

    std::cout << "Genrating Boundaries" << std::endl;
    // --- First clean the Periodic Throats!!!
    if (this->ns->periodicBounndaries)
        this->cleanPeriodic(dir);


//    int Ni = this->ns->Ni;
//    int Nj = this->ns->Nj;
//    int Nk = this->ns->Nk;
//    //int *coord = new int[3];

    int **newTL = nullptr;
    float **newLL = nullptr;
    int **newTC = nullptr;
    float *newPBsize = nullptr;



    // --------- Inlets ---------
    this->nrOfInlets = 0;

    for (size_t i = 1; i <= this->nrOfActivePBs; i++)
        if (this->locationList[dir][i] == 0.0f)
            nrOfInlets++;

    newTL = this->paddedList(nrOfInlets, this->throatList, 2, this->nrOfConnections, true);
    newLL = this->paddedList(nrOfInlets, this->locationList, 3, this->nrOfActivePBs + 1, true);
    newTC = this->paddedList(nrOfInlets, this->throatCounter, 2, this->nrOfActivePBs + 1, true);
    newPBsize = this->paddedList(nrOfInlets, this->pb_sizeList, this->nrOfActivePBs + 1, true);


    size_t bound_index = 1;
    for (size_t i = 1; i <= this->nrOfActivePBs; i++) {
        if (this->locationList[dir][i] == 0.0f) {
            newTL[0][bound_index - 1] = (int) bound_index;
            newTL[1][bound_index - 1] = (int) i + (int) nrOfInlets;

            // Loc_List, the trick here is that the OLD location of the pb to connect to is the location of the boundPB
            newLL[0][bound_index] = this->locationList[0][i];
            newLL[1][bound_index] = this->locationList[1][i];
            newLL[2][bound_index] = this->locationList[2][i];

            newTC[0][bound_index] = 1;

            bound_index++;
        }
    }



    // ------------ Outlets ---------------
    //float lastPbLocs = locationList[dir][this->nrOfActivePBs];
    float lastPbLocs = 0.0;
    for (size_t i = 1; i <= this->nrOfActivePBs; i++)
        lastPbLocs = this->locationList[dir][i] > lastPbLocs ? this->locationList[dir][i] : lastPbLocs;
    //std::cout << "Last Pb location: " << lastPbLocs << " For " << dir << std::endl;


    nrOfOutlets = 0;
    for (size_t i = 1; i <= this->nrOfActivePBs; i++)
        if (this->locationList[dir][i] == lastPbLocs)
            nrOfOutlets++;


    int **tempTL = newTL;
    int **tempTC = newTC;
    float **tempLL = newLL;
    float *tempPbs = newPBsize;

    //Since the padded list is a deep copy of the given list
    newTL = this->paddedList(nrOfOutlets, newTL, 2, this->nrOfConnections + nrOfInlets, false);
    newLL = this->paddedList(nrOfOutlets, newLL, 3, this->nrOfActivePBs + 1 + nrOfInlets, false);
    newTC = this->paddedList(nrOfOutlets, newTC, 2, this->nrOfActivePBs + 1 + nrOfInlets, false);
    newPBsize = this->paddedList(nrOfOutlets, newPBsize, this->nrOfActivePBs + 1 + nrOfInlets, false);

    // delete the old lists
    delete[] tempLL;
    delete[] tempTC;
    delete[] tempTL;
    delete[] tempPbs;

    tempLL = nullptr;
    tempTC = nullptr;
    tempTL = nullptr;
    tempPbs = nullptr;

    size_t transform_TL = nrOfInlets + nrOfConnections - 1;
    size_t transform_P = nrOfInlets + nrOfActivePBs;

    bound_index = 1;//nrOfInlets + this->nrOfActivePBs;

    for (size_t i = 1; i <= this->nrOfActivePBs; i++) {
        if (this->locationList[dir][i] == lastPbLocs) {
            // I is the old pbnr
            newTL[0][bound_index + transform_TL] = (int) i + (int) nrOfInlets; // old pbnr + translation
            newTL[1][bound_index + transform_TL] = (int) (bound_index +
                                                          transform_P); // highest pbnr + ouletIndex = index of outlet

            // The Trick here is the same as with the inlets, BUT we have to add 2 times the pbdist! (once for the inlets and once for the outlets)
            newLL[0][bound_index + transform_P] = this->locationList[0][i];
            newLL[1][bound_index + transform_P] = this->locationList[1][i];
            newLL[2][bound_index + transform_P] = this->locationList[2][i];

            newLL[dir][bound_index + transform_P] += inletThroatLength + outletThroatLength;

            // TC_list
            newTC[0][i + nrOfInlets] += 1;
            newTC[0][bound_index + transform_P] = 0;
            bound_index++;
        }
    }


    // ------------ Transform Inner Network ------------
    size_t j = 0;
    for (size_t i = nrOfInlets; i < this->nrOfConnections + nrOfInlets; i++) {
        // -- update the ThroatList middlePart
        newTL[0][i] += nrOfInlets;
        newTL[1][i] += nrOfInlets;

        // -- register PeriodicThroats
        if (newTL[0][i] > newTL[1][i]) {
            this->periodicThroats[j] = (int) i;
            j++;
        }
    }

    for (size_t i = nrOfInlets + 1; i <= this->nrOfActivePBs + nrOfInlets; i++) {
        // --Translate a PbDistance
        newLL[dir][i] += inletThroatLength;
    }

    // --- Rebuild the accumulators
    int accumulator = 0;
    for (size_t i = 1; i <= this->nrOfActivePBs + nrOfInlets + nrOfOutlets; i++) {
        newTC[1][i] = accumulator + newTC[0][i];
        accumulator = newTC[1][i];
    }


    if (throatCounter) {
        delete[] throatCounter[0];
        throatCounter[0] = nullptr;
        delete[] throatCounter;
        throatCounter = nullptr;
    }
    if (throatList) {
        delete[] throatList[0];
        delete[] throatList;
        throatList = nullptr;
    }
    if (locationList) {
        delete[] locationList[0];
        locationList[0] = nullptr;
        delete[] locationList;
        locationList = nullptr;
    }
    if (pb_sizeList) {
        delete[] pb_sizeList;
        pb_sizeList = nullptr;
    }

    this->throatList = newTL;
    this->throatCounter = newTC;
    this->locationList = newLL;
    this->pb_sizeList = newPBsize;

    if (arr) {
        for (size_t i = 0; i < this->ns->Ni; i++)
            for (size_t j = 0; j < this->ns->Nj; j++) {
                delete[] arr[i][j];
            }
        delete[] arr;
        arr = nullptr;
    }


    // Update nrOfActivePBs
    //std::cout << nrOfActivePBs << '\t' << nrOfConnections << std::endl;

    this->nrOfActivePBs += nrOfInlets + nrOfOutlets;
    this->nrOfConnections += nrOfInlets + nrOfOutlets;

    // Update the Pb_Sizes
    for (size_t i = 0; i < nrOfInlets; i++)
        this->pb_sizeList[this->throatList[0][i]] = this->pb_sizeList[this->throatList[1][i]];

    for (size_t i = nrOfConnections - nrOfOutlets; i < nrOfConnections; i++)
        this->pb_sizeList[this->throatList[1][i]] = this->pb_sizeList[this->throatList[0][i]];

//	for(size_t i = 1; i <= nrOfActivePBs; i++)
//		std::cout << locationList[0][i] << '\t' << locationList[1][i] << '\t' << locationList[2][i] << std::endl;
//		
//    if ( dir == 0 ) {
//        this->ns->Ni += 2;
//    } else if (dir == 1) {
//        this->ns->Nj += 2;
//    } else if (dir == 2) {
//        this->ns->Nk += 2;
//    }
}

/*
 * Flags the Periodic throats and deletes them
 */
void PoreNetwork::cleanPeriodic(size_t flowDir){

    int * coord   = new int[3];
    int * coord_n = new int[3];
    int pn, pn_n;
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    int maxPeriod = Ni*Nj + Nj*Nk + Nk*Ni;

    bool del =  false;
    size_t deleted = 0;



    for (size_t i = 0; i < maxPeriod && this->periodicThroats[i] != 0; i++) {
        //std::cout << "____" << i << "---" << this->periodicThroats[i] << std::endl;
        pn = this->throatList[0][this->periodicThroats[i]];
        pn_n = this->throatList[1][this->periodicThroats[i]];

        deflatten_3d(pn, Ni, Nj, Nk, coord);
        deflatten_3d(pn_n, Ni, Nj, Nk, coord_n);

        if( flowDir == 0 && coord[0] != coord_n[0]){
            del = true;
        }else if( flowDir == 1 && coord[1] != coord_n[1]){
            del = true;
        }else if( flowDir == 2 && coord[2] != coord_n[2]){
            del = true;
        }

        if(del){
            deleted = this->delelteThroat((size_t)this->periodicThroats[i], deleted, -1);
            this->periodicThroats[i] = -1;
            del = false;
        }
    }

    this->removeFlaggedThroats(-1);
    // clean the list
    for (size_t i = 0; i < maxPeriod && this->periodicThroats[i] != 0; i++)
            this->periodicThroats[i] = 0;

    delete [] coord;
    delete [] coord_n;
}


/*
 * Returns a sorted list of throats with connect[0][i] is source pb
 * Array is exactly as long as needed, guard by an entry of [0,0]
 */
size_t PoreNetwork::generateFullConnectivity(){

    int **halfConnectivity = this->throatList;

    std::cout << "Generating Full Connectivity" << std::endl;
    size_t i = 0;
    size_t halfLength = this->nrOfConnections;

    size_t maxConnections = halfLength  * 2+ 1; // Full connectivity is twice the size of half connectivity, and we need one extra place for the [0][0] guards

    //For Details see [generateConnectivity]
    int *t = new int[maxConnections * 2];
    int **connection = new int*[2];
    connection[0] = t;
    connection[1] = t + maxConnections;
	// Shuts memchecker up
	t = nullptr;

//    for(i = 0; i < maxConnections; i++){
//        connection[0][i] = 0;
//        connection[1][i] = 0;
//    }

    for(i = 0; i < halfLength; i++){
        //copy
        connection[0][i * 2] = halfConnectivity[0][i];
        connection[1][i * 2] = halfConnectivity[1][i];


        //swap values and add as well...
        connection[0][i * 2 + 1] = halfConnectivity[1][i];
        connection[1][i * 2 + 1] = halfConnectivity[0][i];

    }
    if(i >= maxConnections*2){
        std::cout << "MEMROY ERROR IN GenConnectivity!" << std::endl;
        std::abort();
    }
    quicksort_iterative(connection, maxConnections - 1);
//    bubbleSortList(connection, maxConnections-1); // Do NOT sort WITH the guards...

    this->throatList_full = connection;

    return maxConnections - 1;
}


///*
// * Returns a sorted list of throats with connect[0][i] is source pb
// * Array is exactly as long as needed, guard by an entry of [0,0]
// */
//size_t PoreNetwork::generateFullConnectivity_NearlySorted() {
//
//    int **halfConnectivity = this->throatList;
//
//    std::cout << "Generating Full Connectivity" << std::endl;
//    size_t halfLength = this->nrOfConnections;
//// Full connectivity is twice the size of half connectivity, and we need one extra place for the [0][0] guards
//    size_t maxConnections = halfLength * 2 + 1;
//
//    //For Details see [generateConnectivity]
//    int *t = new int[maxConnections * 2];
//    int **connection = new int *[2];
//    connection[0] = t;
//    connection[1] = t + maxConnections;
//    // Shuts memchecker up
//    t = nullptr;
//    memset(connection[0], -1, sizeof(int) * maxConnections * 2);
//    size_t k = 0;
//    int thisPb = -1;
//    size_t j;
//
//    for (size_t i = 0; i < halfLength - this->nrOfOutlets; i++) {
//        if (thisPb != halfConnectivity[0][i])
//            for (j = 0; j < i; j++) {
//                if (halfConnectivity[1][j] == halfConnectivity[0][i]) {
//                    connection[0][k] = halfConnectivity[1][j];
//                    connection[1][k] = halfConnectivity[0][j];
//
//                    //DEBUG
//                    std::cout << '*' << k << "\t[" << connection[0][k] << ':' << connection[1][k] << "]\t J:  " << j <<
//                    "\t[" <<
//                    halfConnectivity[1][j] << ':' << halfConnectivity[0][j] << ']' << std::endl;
//
//                    k++;
//                }
//            }
//        thisPb = halfConnectivity[0][i];
//
//        connection[0][k] = halfConnectivity[0][i];
//        connection[1][k] = halfConnectivity[1][i];
//
//        //DEBUG
//        std::cout << '+' << k << "\t[" << connection[0][k] << ':' << connection[1][k] << "]\t I:  " << i <<
//        "\t[" <<
//        halfConnectivity[0][i] << ':' << halfConnectivity[1][i] << ']' << std::endl;
//
//        k++;
//
//    }
//    for(size_t i = halfLength - this->nrOfOutlets - 1; i < halfLength; i++){
//
//        connection[0][k] = halfConnectivity[1][i];
//        connection[1][k] = halfConnectivity[0][i];
//
//        //DEBUG
//        std::cout << '-' << k << "\t[" << connection[0][k] << ':' << connection[1][k] << "]\t I:  " << i <<
//        "\t[" <<
//        halfConnectivity[0][i] << ':' << halfConnectivity[1][i] << ']' << std::endl;
//
//        k++;
//
////        connection[0][k] = halfConnectivity[0][i];
////        connection[1][k] = halfConnectivity[1][i];
////
////        //DEBUG
////        std::cout << '-' << k << "\t[" << connection[0][k] << ':' << connection[1][k] << "]\t I:  " << i <<
////        "\t[" <<
////        halfConnectivity[0][i] << ':' << halfConnectivity[1][i] << ']' << std::endl;
////
////        k++;
//
//    }
//
//    std::cout << "KKKKKK: " << k << std::endl;
//    quicksort_iterative(connection, maxConnections - 1);
////    bubbleSortList(connection, maxConnections - 1); // Do NOT sort WITH the guards...
//
//    this->throatList_full2 = connection;
//
//    return maxConnections - 1;
//}

void PoreNetwork::generatePbSizes() {

    std::cout << "Generating PoreBody Sizes" << std::endl;
    if (!pb_sizeList)
        pb_sizeList = new float[nrOfActivePBs + 1];
    else {
        delete[] pb_sizeList;
        pb_sizeList = new float[nrOfActivePBs + 1];
    }

    if (ns->constantPBSize) {
        for (size_t i = 1; i <= this->nrOfActivePBs; i++)
            pb_sizeList[i] = ns->meanPBsize;
        return;
    }
    for (size_t i = 1; i <= this->nrOfActivePBs; i++)
        pb_sizeList[i] = -1.0;


    std::default_random_engine generator((unsigned int) time(0));
    std::lognormal_distribution<float> distribution(std::log(ns->meanPBsize), ns->stdDev);

    float number;
    size_t i = 1;
    while (i <= nrOfActivePBs) {
        if (!ns->constantPBSize) {
            number = distribution(generator);
            pb_sizeList[i] = randomBetween(ns->minPbSize, ns->maxPbSize, (double) number);
            i++;
        }
    }
}


/*
 * Generate a Coordination number per pb
 * if it is 2 or less, a pb is a dead end.
 * except for inlets and outlets
 */ 
char * PoreNetwork::killDeadEndPores() {

    std::cout << "Killing dead end Pores" << std::endl;

    char *mask;

    bool clean = false;
    int j = 1;
    while (!clean) {
        std::cout << "PASS " << j << std::endl;

        mask = new char[nrOfActivePBs + 1];

        for (size_t i = 0; i <= nrOfActivePBs; i++)
            mask[i] = 0;

        for (size_t i = 1; i <= nrOfInlets; i++)
            mask[i] = 2;

        for (size_t i = nrOfActivePBs - nrOfOutlets; i <= nrOfActivePBs; i++)
            mask[i] = 2;

        for (size_t i = 0; i < this->nrOfConnections; i++) {
            mask[throatList[0][i]]++;
            mask[throatList[1][i]]++;
        }
        // removeFlaggedPBs deletes mask
        if (removeFlaggedPBs(mask, (char) 2) < 1) {
            clean = true;
        }
        j++;

    }
    return mask;
}

/*
 * Delete Pore with PoreNumber i, a flag is placed in the throatlist, and throatcounter is diminished with 1
 * ThroatCounters[pn][1] is now invalid!
 */
size_t PoreNetwork::delelteThroat(size_t i, size_t deleted, int flag){

    this->throatList[1][i] = flag;
    deleted++;
    this->throatCounter[0][this->throatList[0][i]] -= 1;
    this->throatCounter[1][this->throatList[0][i]] = -1;
    return deleted;
}

size_t PoreNetwork::delelteThroat(int i, int deleted, int flag){
    return this->delelteThroat(i, deleted, flag);
}
