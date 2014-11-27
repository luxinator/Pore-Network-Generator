//
//  Generator.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "Generator.h"
#include "ArrayFunctions.h"

#include <iomanip>


void generateLocation(const float Length, int **throatCounters, const int Ni, const int Nj, const int Nk){
    
    
    int *coord = new int[3];
    
    for(int i = 1; i <= Ni*Nj*Nk; i++){
        deflatten_3d(i, Ni, Nj, Nk, coord);
        
        //std::cout << '[' << i << ']'<< ' ';
        std::cout << std::setw(8)<< coord[0] * Length << " ";
        std::cout << std::setw(8)<< coord[1] * Length << " ";
        std::cout << std::setw(8)<< coord[2] * Length << " ";
        std::cout << std::setw(8)<< throatCounters[0][i] << " ";
        std::cout << std::setw(8)<< throatCounters[1][i] << std::endl;
    }
}

std::vector<std::pair<int, int>> *generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array, int **throatCounters){
    
    //Extend this to include variable connection amount based upon distance from Pore to neightbours!
    
    std::vector<std::pair<int, int>> *output= new std::vector<std::pair<int, int>>();     
    int *coord = new int[3];
    int localNr = 0;
    
    if (!array)
        return nullptr;
    
    if(throatCounters == nullptr){
        throatCounters = new int*[2];
        throatCounters[0] = new int[Ni*Nj*Nk];
        throatCounters[1] = new int[Ni*Nj*Nk];
    }
    
    for(int PN = 1; PN <= Ni*Nj*Nk; PN++){
        deflatten_3d(PN, Ni, Nj, Nk,coord);
//        std::cout<< coord[0] << coord[1]<<coord[2] << std::endl;
        
        if (coord[0] < Ni -1){
            output->push_back( std::make_pair(PN, array[coord[0] + 1][coord[1]][coord[2]]));
            throatCounters[0][PN] += 1;
            localNr += 1;
            //std::cout << PN << ' ' << array[coord[0] + 1][coord[1]][coord[2]] << std::endl;
        }
        if (coord[1] < Nj - 1 && coord[0] != 0 && coord[0] != Ni - 1) {
            output->push_back( std::make_pair(PN, array[coord[0]] [coord[1] + 1] [coord[2]]));
            //std::cout << PN << ' ' << array[coord[0]] [coord[1] + 1] [coord[2]] << std::endl;
            throatCounters[0][PN] += 1;
            localNr += 1;
        }
        if (coord[2] < Nk -1 && coord[0] != 0 && coord[0] != Ni - 1){
            output->push_back( std::make_pair(PN, array[coord[0]] [coord[1]] [coord[2] + 1]));
            //std::cout << PN << ' ' << array[coord[0]] [coord[1]] [coord[2] + 1] << std::endl;
            throatCounters[0][PN] += 1;
            localNr += 1;
        }
        throatCounters[1][PN] += localNr;
        
        
    } // For Loop
    
    
    output->shrink_to_fit();
   
    return output;
}
