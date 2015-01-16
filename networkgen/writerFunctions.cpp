#include "writerFunctions.h"

#include <iostream>
#include <fstream>
#include <iomanip>

/*
 * Writes out the throatList until an entry with [0][0] is encounterd
 */
void writeInlet_OutletPbs(const char * filename, PoreNetwork *pn){
    
    
    std::ofstream file;
    if(!filename){
        std::cerr << "No filename specified! " << std::endl;
        return;
    }
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    int *inlets = new int[pn->ns->Ni];
    int *outlets = new int[pn->ns->Ni];

    
    size_t j = 0;
    for( ; j < pn->ns->Ni; j++){
        inlets[j] = 0;
        outlets[j] = 0;
    }


    for(size_t i = 0; i < pn->ns->Ni; i ++){
        if (pn->locationList[0][pn->throatList[0][i]] == 0.0f){ //Assuming inlets are at x = 0
            inlets[j] = pn->throatList[0][i];
            std::cout << inlets[j] << std::endl;
            j++;
        }
    }
    
    for(size_t i = 0; i < pn->ns->Ni; i ++){
        if (pn->locationList[0][pn->throatList[0][i]] == pn->ns->Ni * pn->ns->pbDist){ //Assuming inlets are at x = 0
            outlets[j] = pn->throatList[0][i];
            std::cout << inlets[j] << std::endl;
            j++;
        }
    }
    
    for(size_t i = 0; i < pn->ns->Ni; i ++){
 
    }
    delete[] outlets;
    delete[] inlets;
    
    file.close();
    
}

void writeConnectivity(const char * filename, PoreNetwork *pn){
    
    
    std::ofstream file;
    if(!filename){
        std::cerr << "No filename specified! " << std::endl;
        return;
    }
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    for(size_t i = 0;pn->throatList[0][i] != 0 ; i ++){
            file << pn->throatList[0][i]<< '\t' << pn->throatList[1][i] << std::endl;
    }
    
    file.close();
}

void writeLocation(const char * filename, PoreNetwork *P){
    
    
    
    std::ofstream file;
    if( filename == nullptr){
        std::cerr << "No filename specified! " << std::endl;
        return;
    }
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::trunc);
    if(!file){
        std::cerr<< "Error opening file [" << filename << ']' << std::endl;
        return;
    }
    
    
    // set output type to scientific
    file.setf(std::ios_base::scientific);
    for(size_t pn = 1; pn < P->nrOfActivePBs; pn++){
        
        //file << '[' << pn << ']' << '\t';
        file << std::setw(8)<< P->locationList[0][pn]   << ' ';
        file << std::setw(8)<< P->locationList[1][pn]   << ' ';
        file << std::setw(8)<< P->locationList[2][pn]   << ' ';
        file << std::setw(8)<< P->throatCounter[0][pn] << ' ';
        file << std::setw(8)<< P->throatCounter[1][pn] << '\n';
        //std::cout<< throatCounters[0][pn] << '\t' <<  throatCounters[1][pn] << std::endl;
    }
    
    file.close();
}