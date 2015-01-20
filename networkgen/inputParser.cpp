//
//  inputParser.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 04-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

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

void tolowercase(char *input, const unsigned int size = 0){
    for (int i = 0; i < size; i++) {
        input[i] = tolower(input[i]);
    }
}

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

    std::cout << "Parsing NetworkSpecs: \n\tChanceList:" << std::endl;
    
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
        
        if ( s.compare(0,2,"ni")  == 0) {
            NS->Ni = std::stoi(s.substr(i+1));
        }
        else if ( s.compare(0,2,"nj")  == 0) {
            NS->Nj = std::stoi(s.substr(i+1));
        }
        
        else if( s.compare(0,2,"nk") == 0) {
            NS->Nk = std::stoi(s.substr(i+1));
        }
        else if( s.compare(0,6,"pbdist")  == 0) {
            NS->pbDist = std::stof(s.substr(i+1));
        }
        else if( s.compare(0,8, "periodic") == 0){
            NS->periodicBounndaries = std::stoi(s.substr(i+1)) != 0; // if not zero then true
        }
        // -----
        else if( s.compare(0,1,"c")  == 0) {
            try{
                int pos = std::stoi(s.substr(1,i-1));
                NS->C[pos] = std::stof(s.substr(i+1));
                std::cout <<"\tC[" << pos << "] " << "= " <<NS->C[pos]<<std::endl;
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
    }
    
    std::cout << "NetworkSpecs found: \n\tNi, Nj,Nk:\t\t\t\t\t" << NS->Ni << ",\t" << NS->Nj << ",\t" << NS->Nk << std::endl;
    std::cout << "\tDistance between PoreBodies:\t" << NS->pbDist << '\n';
    std::cout << "\tMaximum Searching Distance: \t" << NS->searchDistance << '\n';
    std::cout << "\tCoordination Number:\t\t\t" << NS->coordNr << '\n';
    std::cout << "\tPeriodic Boundaries:\t\t\t" << NS->periodicBounndaries << std::endl;
    
    
    return NS;
}