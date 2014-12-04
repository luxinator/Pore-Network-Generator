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
#include <fstream>
#include <iomanip>
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




int ** generateConnectivity_2(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters = nullptr){
    
    //Extend this to include variable connection amount based upon distance from Pore to neightbours!
    int *t = new int[2 * 13  *  Ni*Nj*Nk];// 3 connections per pb, we have Ni*Nj*Nk pbs and connection is a pair of 2 ints
    memset(t, 13 *2 * Nj*Nj*Nk, 0);
    int **connection = new int*[2]; //[0] from [1] to
    connection[0] = t;
    connection[1] = t + (13 * Ni*Nj*Nk);
    
    // coords of pb under consideration
    int *coord = new int[3];
    int *coord_n = new int[3];
    
    if (!array)
        return nullptr;
    
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
        
        // boundary inlet
        if (coord[0] == 0){
            connection[0][i] = pn; //Pb nr
            connection[1][i] =  array[coord[0] + 1][coord[1]][coord[2]]; //connected to pb
            throatCounters[0][pn] += 1; //amount of connections of pb
            
            i++;
            
            //std::cout << "inlet: connected!" << '\t' << pn << '-' << array[coord[0] + 1][coord[1]][coord[2]] << std::endl;
            continue; // Skip the rest
        }
        // Connect to Boundary Outlet
        else if (coord[0] == Ni - 2){
            connection[0][i] = pn; //Pb nr
            connection[1][i] =  array[coord[0] + 1][coord[1]][coord[2]]; //connected to pb
            throatCounters[0][pn] += 1; //amount of connections of pb
            
            
            //std::cout << "outlet: connected!" << '\t' << pn << '-' << array[coord[0] + 1][coord[1]][coord[2]] << std::endl;
            i++;
            
        }
        for( int pn_n = pn+1; pn_n < (pn+ Nj*Nk + (Nj*Nk /2 + 1)) && pn_n <= Ni*Nj*Nk - Nj*Nk; pn_n++){
            
            deflatten_3d(pn_n-1, Ni, Nj, Nk, coord_n);
            
            L = sqrt(pow((double)(coord[0] - coord_n[0]), 2.0) +
                     pow((double)(coord[1] - coord_n[1]), 2.0)+
                     pow((double)(coord[2] - coord_n[2]), 2.0));
//            std::cout << pn << '\t' << coord[0] << '\t'<< coord[1] << '\t'<< coord[2] << '\n';
//            std::cout << pn_n<<'\t' << coord_n[0] << '\t'<< coord_n[1] << '\t'<< coord_n[2] << '\n';
            //std::cout<< L << std::endl;
            
            if(L <= sqrt(3.0)){
                connection[0][i] = pn; //Pb nr
                connection[1][i] = pn_n; //connected to pb
                throatCounters[0][pn] += 1; //amount of forward connections of pb
                
                i++;
               // std::cout << "interal: connected!" << '\t' << pn << '-' << pn_n << std::endl;
            }
        } // for
        
        throatCounters[1][pn] += i; //nr of connection made in total
    }// for
    
    delete coord;
    delete coord_n;
    
    return connection;
}
/*
 * Generate a connection to the nearest neighbours of a pb at (i,j,k)
 * Flow is in X-Direction. So the boundary PB's are in i = 1 and i = Ni
 * The list returnd is a list of length 3 * nrPBs
 * at [0] = pn, [1] = pb connected.
 */

int ** generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters = nullptr){
    
    //Extend this to include variable connection amount based upon distance from Pore to neightbours!
    int *t = new int[2 * 3 *  Ni*Nj*Nk];// 3 connections per pb, we have Ni*Nj*Nk pbs and connection is a pair of 2 ints
    memset(t, 3*2*Nj*Nj*Nk, 0);
    int **connection = new int*[2]; //[0] from [1] to
    connection[0] = t;
    connection[1] = t + (3 * Ni*Nj*Nk);
    
    // coords of pb under consideration
    int *coord = new int[3];
    
    if (!array)
        return nullptr;
    
    //throatCounters [0] = amount of connection of pb
    //throatcounters [1] = total nr of connected pb including this one, excluding the ones to come
    if(throatCounters == nullptr){
        int *temp = new int[2 * Ni*Nj*Nk];
        throatCounters = new int*[2];
        throatCounters[0] = temp;
        throatCounters[1] = temp + (Ni*Nj*Nk);
    }
    
    int i = 0;
    
    for(int pn = 1; pn <= Ni*Nj*Nk; pn++){
        deflatten_3d(pn-1, Ni, Nj, Nk, coord);
        
        if (coord[0] < Ni -1){
            // connect every pb in the x-dir except last pores
            connection[0][i] = pn; //Pb nr
            connection[1][i] =  array[coord[0] + 1][coord[1]][coord[2]]; //connected to pb
            throatCounters[0][pn] += 1; //amount of connections of pb

            i++;
        }
        
        // Connect in y- and z-dir
        // If the pb is not part of inlet or outlet boundary!
        if(coord[0] != 0 && coord[0] < Ni - 1){
            
            // If we are not j = N-1 connect in y-dir
            if (coord[1] < Nj - 1) {
                connection[0][i] = pn;
                connection[1][i] = array[coord[0]] [coord[1] + 1] [coord[2]];
                throatCounters[0][pn] += 1;
                
                i++;
            }
            // If we are not k = Nk -1 connect z-dir
            if (coord[2] < Nk - 1){
                connection[0][i] = pn;
                connection[1][i] = array[coord[0]] [coord[1]] [coord[2] + 1];
                throatCounters[0][pn] += 1;

                i++;
            }
            
            //Forward diagonal throat
            if(coord[1] < Nj - 1 && coord[2] < Nk - 1 && coord[0] < Ni - 2){
                connection[0][i] = pn;
                connection[1][i] = array[coord[0]] [coord[1] + 1] [coord[2] + 1];
                throatCounters[0][pn] += 1;
                
                i++;
                
                connection[0][i] = pn;
                connection[1][i] = array[coord[0]+1] [coord[1] + 1] [coord[2] + 1];
                throatCounters[0][pn] += 1;
                
                i++;
                
                connection[0][i] = pn;
                connection[1][i] = array[coord[0]+1] [coord[1]] [coord[2] + 1];
                throatCounters[0][pn] += 1;
                
                i++;
                
            }
            
        }
        
        throatCounters[1][pn] += i; //nr of connection made in total
        
        
    } // For Loop
    
    delete coord;
   
    return connection;
}


void writeConnectivity(const char * filename, int** connect,int nrPB){
    
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
    
    for(int i = 0; i < nrPB * 13; i ++){
        if (connect[0][i] == 0)
            break;
        else
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
    
    file.setf(std::ios_base::scientific);
    for(int pn = 1; pn <= PNMax; pn++){
        
        file << '[' << pn << ']' << '\t';
        file << std::setw(8)<< locationList[0][pn]   << ' ';
        file << std::setw(8)<< locationList[1][pn]   << ' ';
        file << std::setw(8)<< locationList[2][pn]   << ' ';
        file << std::setw(8)<< throatCounters[0][pn] << ' ';
        file << std::setw(8)<< throatCounters[1][pn] << '\n';
    }
    
    file.close();
}