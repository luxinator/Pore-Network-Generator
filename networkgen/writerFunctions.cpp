/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "writerFunctions.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


void writeConnectivity(const char * path, PoreNetwork *pn){

    std::ofstream file;
    if(!path){
        std::cerr << "No filename specified! " << std::endl;
        return;
    }
    std::string filename = std::string(path) + pn->ns->name + "_conn.txt";
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename.c_str(), std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    size_t i, j;
    int periodic = 0;
    for(i = 0; pn->throatList[0][i] != 0 && i < pn->nrOfConnections ; i ++){
        periodic = 0;
        
        for( j = 0; pn->ns->periodicBounndaries && pn->periodicThroats[j] != 0; j ++)
            if (pn->periodicThroats[j] == i)
                periodic = 1;
            
        
        file << pn->throatList[0][i]<< '\t' << pn->throatList[1][i] <<'\t' << periodic << '\n';
    }
    std::cout<< "# nr of throats written to File: " << i << std::endl;
	std::cout << std::endl;
    file.close();
}

void writeLocation(const char * path, PoreNetwork *P){
    
    std::ofstream file;
    if( path == nullptr){
        std::cerr << "No filename specified! " << std::endl;
        return;
    }
    
    std::string filename = std::string(path) + P->ns->name + "_loc.txt";
    
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename.c_str(), std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    size_t pn;
    // set output type to scientific
    file.setf(std::ios_base::scientific);
    for(pn = 1; pn <= P->nrOfActivePBs; pn++){
        
        //file << '[' << pn << ']' << '\t';
        file << std::setw(8)<< P->locationList[0][pn]   << ' ';
        file << std::setw(8)<< P->locationList[1][pn]   << ' ';
        file << std::setw(8)<< P->locationList[2][pn]   << ' ';
        file << std::setw(8)<< P->throatCounter[0][pn] << ' ';
        file << std::setw(8)<< P->throatCounter[1][pn] << '\n';
        //std::cout<< throatCounters[0][pn] << '\t' <<  throatCounters[1][pn] << std::endl;
    }
    
    std::cout << "# PB Locations Writen to file: " << pn -1 << '\n' << std::endl;
    
    file.close();
}


void writeNetworkSpecs(const char * path, PoreNetwork *pn){

    std::ofstream file;
    if( path == nullptr){
        std::cerr << "No filename specified! " << std::endl;
        return;
    }
    
    std::string filename = std::string(path) + pn->ns->name + "_specs.txt";
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename.c_str(), std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    size_t i = 0;
    while (pn->locationList[0][i] == 0.0)
        i++;
    
    file << "Number of PoreBodies = " << pn->nrOfActivePBs << '\n';
    file << "Number of Throats = " << pn->nrOfConnections << '\n';
    file << "Number of InletPBs = " << pn->nrOfInlets << '\n';
    file << "Number of OutletPBs = " << pn->nrOfOutlets << '\n';
    
    file.close();
    
}

void writeInterfacePores(const char * path, PoreNetwork *pn, Combinator *C){
	
	std::ofstream file;
    if( path == nullptr){
        std::cerr << "No filename specified! " << std::endl;
        return;
    }
    
    std::string filename = std::string(path) + pn->ns->name + "_interface.txt";
    
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename.c_str(), std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
	std::vector<std::pair<int,int>> Boundary_Layer = C->getInterface();
	
	for (std::vector<std::pair<int,int>>::iterator it = Boundary_Layer.begin(); it != Boundary_Layer.end(); it++){
		file << it->first << '\t' << it->second << '\n';
	}
	
	file.close();
	
	filename = std::string(path) + pn->ns->name + "_specs.txt";
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename.c_str(), std::ios::app);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
	
	file << "-- Combination -- " << '\n';
	file << "Separation Distance = " << C->getSeparation() << '\n';
	file << "Search Distance = " << C->getSearchDist() << '\n';
	file << "Survival = " << C->getSurvival() << '\n';
	file << "Number of Interface Throats = "<< Boundary_Layer.size() << '\n';
	
	file.close();
	

}