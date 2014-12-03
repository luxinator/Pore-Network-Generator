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
    
    for(int i = 0; i <= Ni*Nj*Nk; i++){
        deflatten_3d(i, Ni, Nj, Nk, coord);
        
        locationList[0][i] = coord[0] * Length;
        locationList[1][i] = coord[1] * Length;
        locationList[2][i] = coord[2] * Length;
        
    }
    
    return locationList;
}




/*
 * Generate a connection to the nearest neighbours of a pb at (i,j,k)
 * Flow is in X-Direction. So the boundary PB's are in i = 1 and i = Ni
 */

connectionList *generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters = nullptr){
    
    //Extend this to include variable connection amount based upon distance from Pore to neightbours!
    
    connectionList *output= new connectionList(); // == std::vector<std::pair<int, int>>()
    int *coord = new int[3];
    int localNr = 0;
    
    if (!array)
        return nullptr;
    
    if(throatCounters == nullptr){
        int *temp = new int[3 * Ni*Nj*Nk];
        throatCounters = new int*[2];
        throatCounters[0] = temp;
        throatCounters[1] = temp + (Ni*Nj*Nk);
    }
    
    for(int i = 0; i <= Ni*Nj*Nk; i++){
        deflatten_3d(i, Ni, Nj, Nk,coord);
        
        if (coord[0] < Ni -1){
            // connect every pb in the x-dir except last pores
            output->push_back( std::make_pair(i, array[coord[0] + 1][coord[1]][coord[2]]));
            throatCounters[0][i] += 1;
            localNr += 1;
        }
        // If the pb is not part of inlet or outlet boundary!
        if(coord[0] != 0 && coord[0] != Ni - 1){
            
            // If we are not j = N-1 connect in y-dir
            if (coord[1] < Nj - 1) {
                output->push_back( std::make_pair(i, array[coord[0]] [coord[1] + 1] [coord[2]]));
                throatCounters[0][i] += 1;
                localNr += 1;
            }
            
            if (coord[2] < Nk - 1){
                output->push_back( std::make_pair(i, array[coord[0]] [coord[1]] [coord[2] + 1]));
                throatCounters[0][i] += 1;
                localNr += 1;
            }
        }
        throatCounters[1][i] += localNr;
        
        
    } // For Loop
    
    
    output->shrink_to_fit();
   
    return output;
}


void writeConnectivity(const char * filename, std::vector<std::pair<int,int>> *connect){
    
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
    
    for(int i = 0; i < connect->size(); i ++){
        file << connect->at(i).first +1<< '\t' << connect->at(i).second +1<< std::endl;
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
    for(int i = 0; i < PNMax; i ++){
        
        file << std::setw(8)<< locationList[0][i]   << ' ';
        file << std::setw(8)<< locationList[1][i]   << ' ';
        file << std::setw(8)<< locationList[2][i]   << ' ';
        file << std::setw(8)<< throatCounters[0][i] << ' ';
        file << std::setw(8)<< throatCounters[1][i] << '\n';
    }
    
    file.close();
}