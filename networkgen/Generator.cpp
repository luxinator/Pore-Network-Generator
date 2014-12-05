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



float ** generateLocation(const float Length, int **throatCounters, const int Ni, const int Nj, const int Nk){
    
    //Allocate a large part of memory
    float *temp = new float[3 * Ni*Nj*Nk];
    // make a pointer to list of 3 pointers to floats
    float **locationList = new float*[3];
    
    // Point at exactly the right places in the Large Part of memory.
    locationList[0] = temp;
    locationList[1] = temp + (Ni*Nj*Nk);
    locationList[2] = temp + (Ni*Nj*Nk*2);
    
    int *coord = new int[3];
    
    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
        deflatten_3d(pn - 1, Ni, Nj, Nk, coord);
        
        locationList[0][pn] = coord[0] * Length;
        locationList[1][pn] = coord[1] * Length;
        locationList[2][pn] = coord[2] * Length;
        
    }
    
    return locationList;
}


/*
 * Generate the Connectivity map based upon the lattice distance between points
 */

int ** generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters = nullptr){
    
    //Extend this to include variable connection amount based upon distance from Pore to neightbours!
    int *t = new int[2 * 13  *  Ni*Nj*Nk];// 3 connections per pb, we have Ni*Nj*Nk pbs and connection is a pair of 2 ints
    memset(t, 0, 13 *2 * Nj*Nj*Nk);
	
    int **connection = new int*[2]; //[0] from [1] to
    connection[0] = t;
    connection[1] = t + (13 * Ni*Nj*Nk);
    
    // coords of pb under consideration
    int *coord = new int[3];
    int *coord_n = new int[3];
    
    if (!array){
        delete[] coord;
		delete[] coord_n;
		return nullptr;
    }
    //throatCounters [0] = amount of connection of pb
    //throatcounters [1] = total nr of connected pb including this one, excluding the ones to come
    if(throatCounters == nullptr){
        int *temp = new int[2 * Ni*Nj*Nk];
        throatCounters = new int*[2];
        throatCounters[0] = temp;
        throatCounters[1] = temp + (Ni*Nj*Nk);
    }
    
    int i = 0;
    double L= 0;
    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
        deflatten_3d(pn-1, Ni, Nj, Nk, coord); //coord from pb[pn]
        
        // boundary inlet only connect in flow dir
        if (coord[0] == 0){
            connection[0][i] = pn; //Pb nr
            connection[1][i] =  array[coord[0] + 1][coord[1]][coord[2]]; //connected to pb
            throatCounters[0][pn] += 1; //amount of connections of pb
            
            i++;
            
            continue; // Skip the rest
        }
        // Connect to Boundary Outlet in x-dir and no more
        else if (coord[0] == Ni - 2){
            connection[0][i] = pn; //Pb nr
            connection[1][i] =  array[coord[0] + 1][coord[1]][coord[2]]; //connected to pb
            throatCounters[0][pn] += 1; //amount of connections of pb
            
            i++;
            
        }
        // (pn+ Nj*Nk + (Nj*Nk /2 + 1) -> distance of one x-slice in x-dir
        //  Ni*Nj*Nk - Nj*Nk -> do not go further then second to last layer in x-dir
        //
        // This can be optimzed! within the same x-plane you have at best 3 throats per pb
        // for the next plane you have at best 9 connections.
        // the lowest pb in the nest x-plane has pn_n = pn + Nj*Nk - Nk - 1
        // the max pn_n = pn + Nj*Nk +Nk + 1
        // By using 2 for loops we lose the need to loop over half of the x-plane in front of pn
        //
        for( int pn_n = pn+1; pn_n < (pn+ Nj*Nk + (Nj*Nk /2 + 1)) && pn_n <= Ni*Nj*Nk - Nj*Nk; pn_n++){
            
            deflatten_3d(pn_n-1, Ni, Nj, Nk, coord_n);
            
            L = sqrt(pow((double)(coord[0] - coord_n[0]), 2.0) +
                     pow((double)(coord[1] - coord_n[1]), 2.0)+
                     pow((double)(coord[2] - coord_n[2]), 2.0));
            
            if(L <= sqrt(2.0)){
                connection[0][i] = pn; //Pb nr
                connection[1][i] = pn_n; //connected to pb
                throatCounters[0][pn] += 1; //amount of forward connections of pb
                
                i++;
                
            }
        } // for
        
        throatCounters[1][pn] += i; //nr of connection made in total
    }// for
    
    delete[] coord;
    delete[] coord_n;
    
    return connection;
}


