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


void trim_chars(char * input, int size, char c){
    
    int i ,j;
    
    for (i = 0; i < size && input[i] != '\0'; i++) {
        if (input[i] == c) {
            for(j = i; j < size-1 && input[j] != '\0'; j++)
                input[j] = input[j + 1];
        }
    }
}

int find_char(char *input, int size, char c){
    int i;
    for (i = 0; i < size && input[i] != '\0'; i++)
        if (input[i] == c)
            return i;
    return -1;
}

void tolowercase(char *input, int size){
    for (int i = 0; i < size; i++) {
        input[i] = tolower(input[i]);
    }
}

NetworkSpecs *readSpecsFile(const char *filename){
    
    NetworkSpecs *NS = new NetworkSpecs;
    
    std::fstream file;
    
    // Insanly bad... need to check against workingdir and do checks... no time, no need!
    if( strcmp(filename, "NetworkSpecs.in")){
        std::cerr << "No Network Specification File"    << std::endl;
        std::cout << "Assuming Standard File location" << std::endl;
    }
    
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
        else if( s.compare(0,6,"length")  == 0) {
            NS->Length = std::stoi(s.substr(i+1));
        }
    }
    
    return NS;
}