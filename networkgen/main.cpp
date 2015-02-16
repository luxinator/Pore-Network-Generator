#include <iomanip>
#include <iostream>


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
    
    std::string nSpecs = "../data/NetworkSpecs.in";
    std::string cFile   = "../data/";
    std::string fcFile   = "../data/";
    std::string lFile   = "../data/";
    
    std::string vtkFile = "../data/";
    
    bool inputWasParsed = false;
    bool writeVTKswitch = false;
    
    
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
    
    
    PoreNetwork *innerNetwork = new PoreNetwork(nSpecs.c_str());
    if(!innerNetwork->ns){
        std::cout<< "Cannot Generate PoreNetwork, please check NetworkSpecs.in for errors" <<std::endl;
        return 1;
    }
    
    std::cout<< "Generating PoreBodies Nrs" << std::endl;
    innerNetwork->generate_naive_array();
    
    
    // --- Generate the Network
    std::cout<< "\nGenerating Network" << std::endl;
    innerNetwork->generateConnectivity();
    innerNetwork->generateLocation();
    
    std::cout << "Eliminating Throats ..." << std::endl;
    eliminateThroats(innerNetwork, 6);
    innerNetwork->removeFlaggedThroats(-1);
    
    // --- Flow Direction Dependent code --- \\
    //      regenarate the connectivity      \\
    //     - Add in Inlet and Outlet Pores   \\
    //     - Restructure the Lists           \\
    //     - Output to Different Files       \\
    
    std::cout << std::endl;
    std::string prefix;
///    std::string specFileDir = specfile;
    
    
    for(int dir = 0; dir <= 2; dir++){
        if(innerNetwork->ns->flowDirs[dir]){
            
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
            
            PoreNetwork *P_Bound = new PoreNetwork(*innerNetwork, prefix + innerNetwork->ns->name);
            
            P_Bound->generateBoundary(dir);
            
            writeVTK(vtkFile.c_str(), P_Bound);
            writeConnectivity(cFile.c_str(), P_Bound);
            
            writeLocation(lFile.c_str(), P_Bound);
            delete P_Bound;
        }
    }
    
    writeVTK(vtkFile.c_str(), innerNetwork);
    writeConnectivity(cFile.c_str(), innerNetwork);
    
    writeLocation(lFile.c_str(), innerNetwork);
    
    
    /*
     std::cout <<"\n";
     P->generateFullConnectivity();
     
     std::cout << "\n";
     char * pb_list = searchForIsolatedPB(P);
     if(!pb_list){
     std::cout << "Network is Broken Aborting" << std::endl;
     return 1;
     }
     
     
     //    writeConnectivity(fcFile.c_str(),P);
     
     // P->removeFlaggedPBs(pb_list, (char)2);
     
     //    writeConnectivity(cFile.c_str(),P);
     
     writeLocation(lFile.c_str(), P);
     
     if(writeVTKswitch)
     writeVTK(vtkFile.c_str(), P);
     
     writeNetworkSpecs(cFile.c_str(), P);
     
     //}
     */
    // }
    
    
    
}




