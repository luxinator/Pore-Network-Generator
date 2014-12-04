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
void writeVTK(const char* filename, int** connect , float** locationList, int Ni, int Nj, int Nk){
    
    
    
    
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
    
    PNMax++;
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
    
//    file << "LINES" << '\t'<<Ni*Nj*Nk << '\t'<< Ni*Nj*Nk * 3 <<std::endl;
//    file << "LINES" << '\t'<< nrOfThroats << '\t'<< nrOfThroats * 3 <<std::endl;
    int i;
    for(i = 0; i < Ni*Nj*Nk * 13; i ++){
        if (connect[0][i] == 0)
            break;
    }
    
    int nrOfThroats = i;
    file << "LINES" << '\t'<< nrOfThroats << '\t'<< nrOfThroats * 3 <<std::endl;
    for(i = 0; i < nrOfThroats; i ++){
                file<< 2<< '\t' << connect[0][i] - 1 << '\t' << connect[1][i] - 1 << std::endl;
        // VTK is zero based zo a line from pb[1] to pb[10] -> p[0] - p[9]
    }
    
    
   
    file << "POINT_DATA" << ' '<<PNMax-1 << '\n'<< "SCALARS size_pb float" << '\n';
    file << "LOOKUP_TABLE default" <<std::endl;
     // Pn Size
    for(int pn = 1; pn < PNMax; pn++){
        file << 1.0f << '\n';
    }
    
    file.close();
    
}