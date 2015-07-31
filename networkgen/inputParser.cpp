//
//  inputParser.cpp
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "inputParser.h"
#include "string.h" //strcmp under Linux is not in <string>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

void trim_chars(char * input, int size, char c){
    
    int i = 0 ,j = 0;
    
    for (i = 0; i < size && input[i] != '\0'; i++) {
        if (input[i] == c) {
            for(j = i; j < size-1 && input[j] != '\0'; j++)
                input[j] = input[j + 1];
        }
    }
}

int find_char(const char *input, const int size, const char c){
    int i;
    for (i = 0; i < size && input[i] != '\0'; i++)
        if (input[i] == c)
            return i;
    return -1;
}

void tolowercase(char *input, const int size = 0){
    for (int i = 0; i < size; i++) {
        input[i] = tolower(input[i]);
    }
}
/*
 * This insane piece of code is to parse
 * the specs file. Be carfull with it!
 */
NetworkSpecs *readSpecsFile(const char *filename){
    
    NetworkSpecs *NS = new NetworkSpecs;
    
    std::fstream file;
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::in);
    if(!file){
        std::cerr<< "Error in opening file [" << filename << ']' << std::endl;
		delete NS;
        return nullptr;
    }
    
    char buff[255];
    char c = ' ';
    int i = 0;

    std::cout << "Parsing NetworkSpecs: " << std::endl;
    
    while(file.getline(buff, 255)){
        //TRIM THE STRING OF SPACES!!!
        trim_chars(buff, 255, c);
        //Convert to Lower Case
        tolowercase(buff, 255);
    
        if(buff[0] == '\n' || buff[0] == '#')
            continue;
        
        i = find_char(buff, 255, '=');
        
        //use the smart ass string function of C++
        std::string s = std::string(buff);
        
        if ( s.compare(0,i,"ni")  == 0) {
            NS->Ni = std::stoi(s.substr(i+1));
        }
        else if ( s.compare(0,i,"nj")  == 0) {
            NS->Nj = std::stoi(s.substr(i+1));
        }
        
        else if( s.compare(0,i,"nk") == 0) {
            NS->Nk = std::stoi(s.substr(i+1));
        }
        else if( s.compare(0,i,"pbdist")  == 0) {
            NS->pbDist = std::stof(s.substr(i+1));
        }
        else if( s.compare(0,i, "periodic") == 0){
            NS->periodicBounndaries = std::stoi(s.substr(i+1)) != 0; // if not zero then true
        }
        else if( s.compare(0,i, "xflow") == 0){
            NS->flowDirs[0] = std::stoi(s.substr(i+1)) != 0; // if not zero then true
        }
        else if( s.compare(0,i, "yflow") == 0){
            NS->flowDirs[1] = std::stoi(s.substr(i+1)) != 0; // if not zero then true
        }
        else if( s.compare(0,i, "zflow") == 0){
            NS->flowDirs[2] = std::stoi(s.substr(i+1)) != 0; // if not zero then true
        } 
        else if( s.compare(0,i, "keepdeadend") == 0){
            NS->keepDeadEnd = std::stoi(s.substr(i+1)) != 0; // if not zero then true
        } 
		else if( s.compare(0,i, "pbsize") == 0){
            NS->pbSizeFile = s.substr(i+1); // if not zero then true
        }
        else if( s.compare(0,i, "pbsizeconstant") == 0){
            NS->constantPBSize = std::stoi(s.substr(i+1)) != 0; // if not zero then true
        }
		else if( s.compare(0,i, "mean") == 0){
            NS->meanPBsize = std::stof(s.substr(i+1)); // if not zero then true
        }
		else if( s.compare(0,i, "stddev") == 0){
            NS->stdDev = std::stof(s.substr(i+1)); // if not zero then true
        }
		else if( s.compare(0,i, "maxpbsize") == 0){
            NS->maxPbSize = std::stof(s.substr(i+1)); // if not zero then true
        }
		else if( s.compare(0,i, "minpbsize") == 0){
            NS->minPbSize = std::stof(s.substr(i+1)); // if not zero then true
        }
        // ----- ChanceList is kind of a special Case
        else if( s.compare(0,1,"c")  == 0) {
            try{
                int pos = std::stoi(s.substr(1,i-1));
                NS->C[pos] = std::stof(s.substr(i+1));
                //std::cout <<"\tC[" << pos << "] " << "= " <<NS->C[pos]<<std::endl;
            } catch (std::exception &e) {
                std::cerr << " ---- ERROR in Parsing the Chance List! " << std::endl;
                return nullptr;
            }
        }
        
        else if( s.compare(0,14,"searchdistance")  == 0) {
            NS->searchDistance = sqrt(std::stod(s.substr(i+1)));
            if(NS->searchDistance >= sqrt(1.0f) && NS->searchDistance < sqrt(2.0f))
                NS->coordNr = 3;
            else if(NS->searchDistance >= sqrt(2.0f) && NS->searchDistance < sqrt(3.0f))
                NS->coordNr = 11;
            else if(NS->searchDistance == sqrt(3.0))
                NS->coordNr = 26;
            else{
                std::cerr << "ERROR: No Valid Search Distance Found!\nOptions are: \n\t 1.0\n\t 2.0  \n\t 3.0 " << std::endl;
                    return nullptr;
                }
        }
        
        else if( s.compare(0,4, "name") == 0) {
            //NS->name = new char[255];
            NS->name = s.substr(i+1);

        }
    }
    std::cout << "\nname: " << NS->name << std::endl;
    std::cout << "NetworkSpecs found: \n\tNi, Nj,Nk:\t\t\t\t\t" << NS->Ni << ",\t" << NS->Nj << ",\t" << NS->Nk << std::endl;
    std::cout << "\tDistance between PoreBodies:\t" << NS->pbDist << '\n';
    std::cout << "\tMaximum Searching Distance: \t" << NS->searchDistance << '\n';
    std::cout << "\tCoordination Number:\t\t\t" << NS->coordNr << '\n';
    std::cout << "\tPeriodic Boundaries:\t\t\t" << NS->periodicBounndaries << '\n';
    std::cout << "\tPossible Flow Dir:\n\t\t\tX-Flow: " << NS->flowDirs[0] << " Y-Flow: " << NS->flowDirs[1] << " Z-Flow: " << NS->flowDirs[2] << '\n';
    std::cout << "\tPoreBody Sizes are LogNormal Distributed: " << !NS->constantPBSize << '\n';

    std::cout << std::endl;
    
    file.close();
    
    return NS;
}

int loadNrs(const char *filename, PoreNetwork *P){
    
    std::fstream file;
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::in);
    if(!file){
        std::cerr<< "Error in opening file [" << filename << ']' << std::endl;
        return -1;
    }
    if(!P){
        std::cerr << "Error: No PoreNetwork Object Supplied! Check Input Files" << std::endl;
        return -2;
    }
    
    const int buffsize = 255;
    char buff[buffsize];
    char c = ' ';
    int i = 0;
    
    while(file.getline(buff, buffsize)){
        //TRIM THE STRING OF SPACES!!!
        trim_chars(buff, buffsize, c);
        //Convert to Lower Case
        tolowercase(buff, buffsize);
        
        if(buff[0] == '\n' || buff[0] == '#')
            continue;
        
        i = find_char(buff, buffsize, '=');
        
        //use the smart ass string function of C++
        std::string s = std::string(buff);
      
        if ( s.compare(0,18,"numberofporebodies")  == 0) {
            P->nrOfActivePBs = std::stoi(s.substr(i+1));
        }
        else if ( s.compare(0,15,"numberofthroats")  == 0) {
            P->nrOfConnections = std::stoi(s.substr(i+1));
        }
        else if ( s.compare(0,16,"numberofinletpbs")  == 0) {
            P->nrOfInlets = std::stoi(s.substr(i+1));
        }
        else if ( s.compare(0,17,"numberofoutletpbs")  == 0) {
            P->nrOfOutlets = std::stoi(s.substr(i+1));
        }
    }
    
    file.close();
	return 0;
}

int loadPoreBodyLocations(const char *filename, PoreNetwork *P){
    std::ifstream file;
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::in);
    if(!file){
        std::cerr<< "Error in opening file [" << filename << ']' << std::endl;
        return -1;
    }
   
    float x, y, z, pbsize;
    int counter, accu;
    int i = 1;
    
    while (file >> x >> y >> z >> counter >> accu >> pbsize && i <= P->nrOfActivePBs){

        P->locationList[0][i] = x;
        P->locationList[1][i] = y;
        P->locationList[2][i] = z;
        
        P->throatCounter[0][i] = counter;
        P->throatCounter[1][i] = accu;
        
		P->pb_sizeList[i] = pbsize;
        //std::cout << P->locationList[0][i] << '\t'<< P->locationList[1][i] << '\t'<< P->locationList[2][i] << std::endl;
        
        i++;
    }
	
	return 0;
    
}

int loadThroats(const char *filename, PoreNetwork *P){
    std::ifstream file;
    
    std::cout << "Opening File: " << filename << std::endl;
    file.open(filename, std::ios::in);
    if(!file){
        std::cerr<< "Error in opening file [" << filename << ']' << std::endl;
        return -1;
    }
    if(!P){
        std::cerr << "Error: No PoreNetwork Object Supplied! Check Input Files" << std::endl;
        return -1;
    }
    
    int from, to, periodic;
    int i = 0, j = 0;
    
    while (file >> to >> from >> periodic && i < P->nrOfConnections){

        
        if (periodic) {
            P->periodicThroats[j] = i;
            j++;
        }
        P->throatList[0][i] = to;
        P->throatList[1][i] = from;
       // std::cout << i << '\t' << P->throatList[0][i] << '\t' << P->throatList[1][i]<< std::endl;
        i++;
        
    }
	
	return 0;
    
}










