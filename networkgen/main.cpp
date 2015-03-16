/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
 

#include <iomanip>
#include <iostream>
#include <fstream>

#include "ArrayFunctions.h"
#include "vtk.h"
#include "inputParser.h"
#include "Eliminator.h"
#include "writerFunctions.h"

int main(int argc, char *argv[]) {
    
    std::cout << "\nPore Network Generator Compiled at " << __DATE__ << ' ' << __TIME__<<std::endl;
    std::cout << "Copyright Lucas van Oosterhout. All Rights Reserverd. \n\n" << std::endl;
    
    std::string helpText = "Command-Line Options:\n \
    -h               \t Shows this help text\n \
    -ns [location]   \t Specify the location of the NetworkSpecs.in file\n \
    \t if not given standard location is used [../data/NetworkSpecs.in]\n \
    -cfile [location] \t Specify the location of the connectiviy output file\n \
    -fcfile [location]\t Specify the location of the fullconnectivity output file\n \
    -lfile [location] \t Specify the location of the locations file\n \
    -vtk [location]   \t Specify if a vtk file is to be written and where\n";
    
    std::string nSpecs = "./data/NetworkSpecs.in";
    std::string cFile   = "./data/";
    std::string fcFile   = "./data/";
    std::string lFile   = "./data/";
    
    std::string vtkFile = "./data/";
 
    std::string name = "";
    
    bool inputWasParsed = false;
    bool writeVTKswitch = false;
    bool generate = true;
    
    
    // Go through the list of given args
    if (argc > 1) {
        for(int i = 1; i < argc; i++){
            std::string s = std::string(argv[i]);
            
            if(s.compare(0,2,"-h") == 0){
                std::cout<< helpText << std::endl;
                return 0;
            }
            else if(s.compare(0,3, "-ns") == 0){
                inputWasParsed = true;
                nSpecs = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,6, "-cfile") == 0){
                inputWasParsed = true;
                cFile = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,7, "-fcfile") == 0){
                inputWasParsed = true;
                fcFile = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,4, "-vtk") == 0){
                inputWasParsed = true;
                vtkFile = std::string(argv[i+1]);
                writeVTKswitch = true;
                i++;
            }
            else if(s.compare(0,6, "-lfile") == 0){
                inputWasParsed = true;
                lFile = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,6, "-inner") == 0){
                inputWasParsed = true;
                generate = false;
                name = std::string(argv[i+1]);
                i++;
            } else
                inputWasParsed = false;
        }
        if(!inputWasParsed){
            std::cout << "Error Parsing Input, run with -h for help" << std::endl;
            return 1;
        }
    } else {
        std::cout << "No Arguments given, assuming default behavior" << std::endl;
    }
    
    if(generate){
        PoreNetwork *innerNetwork = new PoreNetwork(nSpecs.c_str());
        if(!innerNetwork->ns){
            std::cout<< "Cannot Generate PoreNetwork, please check NetworkSpecs.in for errors" <<std::endl;
            return 1;
        }
        
        std::cout<< "Generating PoreBodies Nrs" << std::endl;
        innerNetwork->generate_naive_array();
        
        
        // --- Generate the innerNetwork
        std::cout<< "\nGenerating Network" << std::endl;
        innerNetwork->generateConnectivity();
        innerNetwork->generateLocation();
        
        std::cout << "Eliminating Throats ..." << std::endl;
        eliminateThroats(innerNetwork, 6);
        innerNetwork->removeFlaggedThroats(-1);
        
        writeVTK(vtkFile.c_str(), innerNetwork);
        writeConnectivity(cFile.c_str(), innerNetwork);
        
        writeLocation(lFile.c_str(), innerNetwork);
        writeNetworkSpecs(cFile.c_str(), innerNetwork);
        

        std::cout << std::endl;
        std::string suffix;
        
        for(int dir = 0; dir <= 2; dir++){
            if(innerNetwork->ns->flowDirs[dir]){
                
                switch (dir) {
                    case 0:
                        suffix = "_x";
                        break;
                        
                    case 1:
                        suffix = "_y";
                        break;
                        
                    case 2:
                        suffix = "_z";
                        break;
                }
                
                PoreNetwork *P_Bound = new PoreNetwork(*innerNetwork, innerNetwork->ns->name + suffix);
                
                P_Bound->generateBoundary(dir);
                
                size_t lengthTL = P_Bound->generateFullConnectivity();
                
                char * pb_list = searchForIsolatedPB(P_Bound,lengthTL);
                if(!pb_list){
                    std::cout << "Network is Broken Aborting" << std::endl;
                    return 1;
                }
                
                P_Bound->removeFlaggedPBs(pb_list, (char)2);

                writeVTK(vtkFile.c_str(), P_Bound);
                writeConnectivity(cFile.c_str(), P_Bound);
                
                writeLocation(lFile.c_str(), P_Bound);
                writeNetworkSpecs(cFile.c_str(), P_Bound);
                
                delete P_Bound;
                std::cout << std::endl;
            }
        }
              
        delete innerNetwork;
    }
    
    else{
        
        name =  "/Users/lucas/Programming/Xcode/PoreNetworkGen/connectivityGenV2/pore-network/data/valid_error_network/testnetwork1";
        
        PoreNetwork * inner = new PoreNetwork(name , "/Users/lucas/Programming/Xcode/PoreNetworkGen/connectivityGenV2/pore-network/data/valid_error_network/NetworkSpecs.in");
        
        std::string nSpecs = "/Users/lucas/Programming/Xcode/PoreNetworkGen/connectivityGenV2/pore-network/data/valid_error_network/";
        std::string cFile   = "/Users/lucas/Programming/Xcode/PoreNetworkGen/connectivityGenV2/pore-network/data/valid_error_network/";
        std::string fcFile   = "/Users/lucas/Programming/Xcode/PoreNetworkGen/connectivityGenV2/pore-network/data/valid_error_network/";
        std::string lFile   = "/Users/lucas/Programming/Xcode/PoreNetworkGen/connectivityGenV2/pore-network/data/valid_error_network/";
        
        std::string vtkFile = "/Users/lucas/Programming/Xcode/PoreNetworkGen/connectivityGenV2/pore-network/data/valid_error_network/";
        

        
        std::string prefix;
        
        writeVTK(vtkFile.c_str(), inner);

        
        for(int dir = 0; dir <= 2; dir++){
            if(inner->ns->flowDirs[dir]){
                
                switch (dir) {
                    case 0:
                        prefix = "x_";
                        break;
                        
                    case 1:
                        prefix = "y_";
                        break;
                        
                    case 2:
                        prefix = "z_";
                        break;
                }
                
                PoreNetwork *P_Bound = new PoreNetwork(*inner, prefix + inner->ns->name);
                
                P_Bound->generateBoundary(dir);
                
                size_t lengthTL = P_Bound->generateFullConnectivity();
                
                char * pb_list = searchForIsolatedPB(P_Bound,lengthTL);
        
                {
                    std::ofstream file;
                    file.open((vtkFile + "pb_flags.txt"), std::ios::trunc);
                    
                    for(size_t i = 1; i <= P_Bound->nrOfActivePBs; i++)
                        file << (int)pb_list[i] << std::endl;
                }
                
                if(!pb_list){
                    std::cout << "Network is Broken Aborting" << std::endl;
                    return 1;
                }
                
                P_Bound->removeFlaggedPBs(pb_list, (char)2);
                
                
                writeVTK(vtkFile.c_str(), P_Bound);
                writeConnectivity(cFile.c_str(), P_Bound);
                
                writeLocation(lFile.c_str(), P_Bound);
                writeNetworkSpecs(cFile.c_str(), P_Bound);
                
                delete P_Bound;
            }
        }

        
        
        
        
    }
    
}




