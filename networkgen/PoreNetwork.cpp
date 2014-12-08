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


void writeConnectivity(const char * filename, int** connect,int nrPB){
    
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