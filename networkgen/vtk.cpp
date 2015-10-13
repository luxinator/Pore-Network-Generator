//
// Created by lucas on 4/16/15.
//

#include "vtk.h"


//
//  vtk.cpp
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "vtk.h"
#include <iostream>
#include <iomanip>
#include <fstream>


void writeCoord(std::ofstream &file, const PoreNetwork *P_net){

    size_t PNMax = P_net->nrOfActivePBs;

    file << '\n' << "SCALARS x-coord float" << '\n';
    file << "LOOKUP_TABLE default" << std::endl;
    // Pn Size
    for (size_t pn = 1; pn <= PNMax; pn++)
        file << (float) P_net->locationList[0][pn] << '\n';

    file <<  '\n' << "SCALARS y-coord float" << '\n';
    file << "LOOKUP_TABLE default" << std::endl;
    // Pn Size
    for (size_t pn = 1; pn <= PNMax; pn++)
        file << (float) P_net->locationList[1][pn] << '\n';


    file <<  '\n' << "SCALARS z-coord float" << '\n';
    file << "LOOKUP_TABLE default" << std::endl;
    // Pn Size
    for (size_t pn = 1; pn <= PNMax; pn++)
        file << (float) P_net->locationList[2][pn] << '\n';
}

/*
 * Write current network to vtk file,
 * Enhance the shit out of this, we want te select what to put in the files
 * Or even better put any and everything in the file!
 */

void writeVTK(std::string filename, const PoreNetwork *P_net, double * pb_values, bool doCoords,
              const int precision, int offset_start, int offset_end){

    size_t PNMax = P_net->nrOfActivePBs;
    std::ofstream file;
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file.is_open()){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }

    //Set output Format
    file.setf(std::ios_base::scientific );
    if(precision)
        file.precision(precision);

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
    for(size_t pn = 1; pn <= PNMax; pn++){
        file << P_net->locationList[0][pn]<< '\t';
        file << P_net->locationList[1][pn]<< '\t';
        file << P_net->locationList[2][pn] << '\n';
    }


    /*
     * Write throat data
     */

    file << "LINES" << '\t'<< P_net->nrOfConnections << '\t'<<  P_net->nrOfConnections * 3 <<std::endl;
    for(size_t i = 0; i <  P_net->nrOfConnections; i++){
        // VTK is zero based zo a line from pb[1] to pb[10] -> p[0] - p[9]
        file << 2 << '\t' << P_net->throatList[0][i] - 1 << '\t' << P_net->throatList[1][i] - 1 << '\n';
    }

    /*
     * Write Point Data if any
     */

    if( pb_values ){
        file << "POINT_DATA" << ' '<<PNMax << '\n'<< "SCALARS size_pb float" << '\n';
        file << "LOOKUP_TABLE default" << std::endl;
        // Pn Size
        for(size_t pn = 1; pn <= PNMax; pn++){
            if(pn < offset_start)
                file << 0.0f << '\n';
            else if(offset_end && pn >= offset_end)
                file << 0.0f << '\n';
            else
                file << pb_values[pn] << '\n';
            //std::cout<< pn << '\t' << (float)(int)pb_flags[pn] << std::endl;
        }
    }
    if(doCoords) {
        writeCoord(file, P_net);
    }
    file.close();

}

void writeVTK(std::string filename, const PoreNetwork *P_net, float * pb_values, bool doCoords,
              const int precision, int offset_start, int offset_end){

    size_t PNMax = P_net->nrOfActivePBs;
    std::ofstream file;
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file.is_open()){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }

    //Set output Format
    file.setf(std::ios_base::scientific );
    if(precision)
        file.precision(precision);

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
    for(size_t pn = 1; pn <= PNMax; pn++){
        file << P_net->locationList[0][pn]<< '\t';
        file << P_net->locationList[1][pn]<< '\t';
        file << P_net->locationList[2][pn] << '\n';
    }


    /*
     * Write throat data
     */

    file << "LINES" << '\t'<< P_net->nrOfConnections << '\t'<<  P_net->nrOfConnections * 3 <<std::endl;
    for(size_t i = 0; i <  P_net->nrOfConnections; i++){
        // VTK is zero based zo a line from pb[1] to pb[10] -> p[0] - p[9]
        file << 2 << '\t' << P_net->throatList[0][i] - 1 << '\t' << P_net->throatList[1][i] - 1 << '\n';
    }

    /*
     * Write Point Data if any
     */

    if( pb_values ){
        file << "POINT_DATA" << ' '<<PNMax << '\n'<< "SCALARS size_pb float" << '\n';
        file << "LOOKUP_TABLE default" << std::endl;
        // Pn Size
        for(size_t pn = 1; pn <= PNMax; pn++){
            if(pn < offset_start)
                file << 0.0f << '\n';
            else if(offset_end && pn >= offset_end)
                file << 0.0f << '\n';
            else
                file << pb_values[pn] << '\n';
            //std::cout<< pn << '\t' << (float)(int)pb_flags[pn] << std::endl;
        }
    }
    if(doCoords) {
        writeCoord(file, P_net);
    }
    file.close();

}

void writeVTK(std::string filename, const PoreNetwork *P_net, bool * pb_values, bool doCoords,
              const int precision, int offset_start, int offset_end){

    size_t PNMax = P_net->nrOfActivePBs;
    std::ofstream file;
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file.is_open()){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }

    //Set output Format
    file.setf(std::ios_base::scientific );
    if(precision)
        file.precision(precision);

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
    for(size_t pn = 1; pn <= PNMax; pn++){
        file << P_net->locationList[0][pn]<< '\t';
        file << P_net->locationList[1][pn]<< '\t';
        file << P_net->locationList[2][pn] << '\n';
    }


    /*
     * Write throat data
     */

    file << "LINES" << '\t'<< P_net->nrOfConnections << '\t'<<  P_net->nrOfConnections * 3 <<std::endl;
    for(size_t i = 0; i <  P_net->nrOfConnections; i++){
        // VTK is zero based zo a line from pb[1] to pb[10] -> p[0] - p[9]
        file << 2 << '\t' << P_net->throatList[0][i] - 1 << '\t' << P_net->throatList[1][i] - 1 << '\n';
    }

    /*
     * Write Point Data if any
     */

    if( pb_values ){
        file << "POINT_DATA" << ' '<<PNMax << '\n'<< "SCALARS size_pb float" << '\n';
        file << "LOOKUP_TABLE default" << std::endl;
        // Pn Size
        for(size_t pn = 1; pn <= PNMax; pn++){
            if(pn < offset_start)
                file << 0.0f << '\n';
            else if(offset_end && pn >= offset_end)
                file << 0.0f << '\n';
            else
                file << (float) pb_values[pn] << '\n';
            //std::cout<< pn << '\t' << (float)(int)pb_flags[pn] << std::endl;
        }
    }
    if(doCoords) {
        writeCoord(file, P_net);
    }		file << 1.0 << '\n';

    file.close();

}


void writeVTK(std::string filename, const PoreNetwork *P_net, char * pb_values, bool doCoords,
              const int precision, int offset_start, int offset_end){

    size_t PNMax = P_net->nrOfActivePBs;
    std::ofstream file;
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file.is_open()){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }

    //Set output Format
    file.setf(std::ios_base::scientific );
    if(precision)
        file.precision(precision);

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
    for(size_t pn = 1; pn <= PNMax; pn++){
        file << P_net->locationList[0][pn]<< '\t';
        file << P_net->locationList[1][pn]<< '\t';
        file << P_net->locationList[2][pn] << '\n';
    }


    /*
     * Write throat data
     */

    file << "LINES" << '\t'<< P_net->nrOfConnections << '\t'<<  P_net->nrOfConnections * 3 <<std::endl;
    for(size_t i = 0; i <  P_net->nrOfConnections; i++){
        // VTK is zero based zo a line from pb[1] to pb[10] -> p[0] - p[9]
        file << 2 << '\t' << P_net->throatList[0][i] - 1 << '\t' << P_net->throatList[1][i] - 1 << '\n';
    }

    /*
     * Write Point Data if any
     */

    if( pb_values ){
        file << "POINT_DATA" << ' '<<PNMax << '\n'<< "SCALARS size_pb float" << '\n';
        file << "LOOKUP_TABLE default" << std::endl;
        // Pn Size
        for(size_t pn = 1; pn <= PNMax; pn++){
            if(pn < offset_start)
                file << 0.0f << '\n';
            else if(offset_end && pn >= offset_end)
                file << 0.0f << '\n';
            else
                file << (float) pb_values[pn] << '\n';
            //std::cout<< pn << '\t' << (float)(int)pb_flags[pn] << std::endl;
        }
    }

    if(doCoords) {
        writeCoord(file, P_net);
    }


    file.close();

}



//void writeVTK(std::string filename, const PoreNetwork *P_net) {
//    double * empty = nullptr;
//    writeVTK(filename, P_net, empty);
//}