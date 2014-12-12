//
//  Generator.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "Generator.h"
#include "ArrayFunctions.h"

#include <iostream>
#include <math.h>



float ** generateLocation(const int Ni, const int Nj, const int Nk, PoreNetwork *P_net){
    
    float Length = P_net->ns->Length;
    
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
    P_net->locationList = locationList;
    
    return locationList;
}


/*
 * return: connect[0][i] = source pb -> connect[1][i] = destination
 * Generate a half Connectivity map based upon the lattice distance between points.
 * The array return is huge and bulky.
 */

int ** generateConnectivity(const int Ni, const int Nj, const int Nk, PoreNetwork *P_net){
    
    if (!P_net->arr){
        std::cerr << "No pb Array supplied to generateConnectivity!" << std::endl;
        return nullptr;
    }
    int *** array = P_net->arr;
    
    // --- coords of pb under consideration
    int *coord = new int[3];
    int *coord_n = new int[3];
    
    // --- Init throatCounters arrays

    //Allocate a Part of Memory
    int *t = new int[2 * Ni*Nj*Nk + 2];
    P_net->throatCounter = new int*[2];
    //Point the two pointers to their places in the Large Part of memory
    P_net->throatCounter[0] = t;
    P_net->throatCounter[1] = t + Ni*Nj*Nk + 1;
    
    // Make a local variable
    int ** throatCounters = P_net->throatCounter;
    
    
    
    // --- Extend this to include variable connection amount based upon distance from Pore to neightbours!
    t = new int[2 * 13  *  Ni*Nj*Nk];// 13 connections per pb, we have Ni*Nj*Nk pbs and connection is a pair of 2 ints
    
    int **connection = new int*[2]; //[0] from [1] to
    connection[0] = t;
    connection[1] = t + (13 * Ni*Nj*Nk);
    
    for(int i = 0; i < 13  *  Ni*Nj*Nk; i ++){
        connection[0][i] = 0;
        connection[1][i] = 0;
    }
    // Add to P_net
    P_net->throatList = connection;
    
    
    std::cout << "number of PBs: "<<2 * Ni*Nj*Nk << std::endl;
    
    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
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
    
    return connection;
}

/*
 * Returns a sorted list of throats with connect[0][i] is source pb
 * Array is exactly as long as needed, will output length to *length
 */
int **generateFullConnectivity(const int Ni, const int Nj, const int Nk,PoreNetwork *P_net){
    
    int **halfConnectivity = P_net->throatList;
    
    std::cout << "Generating Full Connectivity" << std::endl;
    int i = 0;
    int halfLength = 0;
    //maximum number of connections is:
    for(i = 0; i < Ni*Nj*Nk * 13; i++){
        if(halfConnectivity[0][i] == 0){
            halfLength = i;
            break;
        }
    }
    int maxConnections = halfLength  * 2 +1; // Full connectivity is twich the size of half connectivity, and we need one extra place for the [0][0] guards
    
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

    
    P_net->throatList_full = connection;
    return connection;
}

