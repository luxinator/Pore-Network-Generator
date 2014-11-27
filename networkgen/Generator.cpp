//
//  Generator.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 26-11-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "Generator.h"
#include "ArrayFunctions.h"


location * generateLocation(const std::vector<std::pair<int, int>> * connectivity, const int Ni, const int Nj, const int Nk, const float Length){
    
    //Locations are a list of x,y,z values with a connection count (frequency) and accumulator
    // accumulator is the amount of pore assigned including the one being assigned
    location *output = new location[Ni*Nj*Nk];
    
    int nrOfThroats = 0, currentNrOfThroats = 0;
    std::cout << "Starting Location Generation" << std::endl;
    int *t = new int[3]; // Coordinate list
    
    //Pore numbering starts at 1!
    for(int i = 1; i < connectivity->size(); i++){
        for(int j = i; j < connectivity->size(); j++){
            
            if( connectivity->at(i).first != connectivity->at(j).first){
                //Encounterd different Pore
                
                currentNrOfThroats = j - i;
                nrOfThroats += currentNrOfThroats;
                
               deflatten_3d(i, Ni, Nj, Nk, t);
                
                output[i] = location(t[0] * Length, t[1] * Length, t[2] * Length,
                    currentNrOfThroats, nrOfThroats);
                
                currentNrOfThroats = 0; //reset counter for new pore
                break;
                
            } // if
        } //J - Loop
        if(i % 250 == 0)
            std::cout << "At PN: "<<i <<std::endl;
    } // I - Loop
    
    return output;
    
}

std::vector<std::pair<int, int>> *generateConnectivity(const int Ni, const int Nj, const int Nk, int ***array){
    
    //Extend this to include variable connection amount based upon distance from Pore to neightbours!
    
    std::vector<std::pair<int, int>> *output= new std::vector<std::pair<int, int>>();     
    int *coord = new int[3];
    
    if (array == nullptr)
        array = generate_naive_array(Ni, Nj, Nk);
    
    for(int PN = 1; PN <= Ni*Nj*Nk; PN++){
        deflatten_3d(PN, Ni, Nj, Nk,coord);
//        std::cout<< coord[0] << coord[1]<<coord[2] << std::endl;
        
        if (coord[0] < Ni -1){
            output->push_back( std::make_pair(PN, array[coord[0] + 1][coord[1]][coord[2]]));
            //std::cout << PN << ' ' << array[coord[0] + 1][coord[1]][coord[2]] << std::endl;
        }
        if (coord[1] < Nj - 1 && coord[0] != 0 && coord[0] != Ni - 1) {
            output->push_back( std::make_pair(PN, array[coord[0]] [coord[1] + 1] [coord[2]]));
            //std::cout << PN << ' ' << array[coord[0]] [coord[1] + 1] [coord[2]] << std::endl;
        }
        if (coord[2] < Nk -1 && coord[0] != 0 && coord[0] != Ni - 1){
            output->push_back( std::make_pair(PN, array[coord[0]] [coord[1]] [coord[2] + 1]));
            //std::cout << PN << ' ' << array[coord[0]] [coord[1]] [coord[2] + 1] << std::endl;
        }
        
    }
    output->shrink_to_fit();
    
    return output;
}