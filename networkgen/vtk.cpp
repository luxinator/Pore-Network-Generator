//
//  vtk.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 02-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "vtk.h"
#include <iostream>
#include <iomanip>
#include <fstream>


/*
 * Write current network to vtk file
 */
void writeVTK(const char* filename, connectionList* connect , float** locationList, int Ni, int Nj, int Nk){
    
    
    
    
    int PNMax = Ni*Nj*Nk;
    
    std::ofstream file;
    if( filename == nullptr){
        std::cerr << "Invalid filename specified! " << std::endl;
    }
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    file.setf(std::ios_base::scientific );
    
    /*
     * Write the header
     */
    file << "# vtk DataFile Version 3.0"     << std::endl;
    file << "pore-scale field varables"     << std::endl;
    file << "ASCII"                         << std::endl;
    file << "DATASET POLYDATA "             << std::endl;
    file << "POINTS "<<'\t' << PNMax <<'\t'<< "FLOAT"   << std::endl;
    
    
    
    /*
     * Write pb location data
     */
    for(int pn = 1; pn < PNMax; pn++){
        file << locationList[0][pn]<< '\t';
        file << locationList[1][pn]<< '\t';
        file << locationList[2][pn] << '\n';
        
    }
    
    /*
     * Write throat data
     */
    file << "LINES" << '\t'<<PNMax << '\t'<<PNMax * 3 <<std::endl;
    
    for(int pn = 1; pn < PNMax; pn++){
        file << 2 << '\t' << connect->at(pn).first -1<< '\t' << connect->at(pn).second -1 << '\n';
    }
    
    
    file << "POINT_DATA" << ' '<<PNMax << '\n'<< "SCALARS size_pb float" << '\n';
    file << "LOOKUP_TABLE default" <<std::endl;
    
    for(int pn = 1; pn < PNMax; pn++){
        file << 2.0e-2<< '\n';
    }

    file.close();
    
}