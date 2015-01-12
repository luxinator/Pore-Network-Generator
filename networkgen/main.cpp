#include <iomanip>
#include <iostream>


#include "ArrayFunctions.h"
#include "vtk.h"
#include "inputParser.h"
#include "Eliminator.h"


int main(int argc, char *argv[]) {
    
    std::cout << "\nPore Network Generator Compiled at " << __DATE__ << ' ' << __TIME__<<std::endl;
    std::cout << "Copyright Lucas van Oosterhout. All Rights Reserverd. \n\n" << std::endl;

    std::string helpText = "Command-Line Options: \n \
    -h:               \t Shows this help text \n \
    -ns [location]:   \t Specify the location of the NetworkSpecs.in file \n \
                      \t if not given standard location is used [../data/NetworkSpecs.in]\n \
    -cfile [location]: \t Specify the location of the connectiviy.txt output file\n \
    -fcfile [location]:\t Specify the location of the fullconnectivity.txt output file\n \
    -lfile [location]: \t Specify the location of the locations.txt file\n \
    -vtk [location]:   \t Specify if the vtk file is to be written and where";
    
    std::string nSpecs = "../data/NetworkSpecs.in";
    std::string cFile   = "../data/connectivity.txt";
    std::string fcFile   = "../data/connectivity.txt";
    std::string lFile   = "../data/location.txt";
    
    std::string vtkFile = "../data/data.vtk";
    
    bool inputWasParsed = false;
    bool writeVTKswitch = false;
    
    // Go through the list ofgiven args
    if (argc > 1) {
        for(int i = 1; i < argc; i++){
            std::string s = std::string(argv[i]);
            
            if(s.compare(0,2,"-h") == 0){
                std::cout<< helpText << std::endl;
                return 0;
                }
            else if(s.compare(0,3, "-ns") == 0){
                //if(argc % 2 != 0){
                //    std::cout << "Error Parsing Input:\nNo Location Specified!" << std::endl;
                //    return -1;
                //}
                inputWasParsed = true;
                nSpecs = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,6, "-cfile") == 0){
                inputWasParsed = true;
                cFile = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,6, "-fcfile") == 0){
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
    }
    
    PoreNetwork *P = new PoreNetwork(nSpecs.c_str());
    if(!P->ns){
        std::cout<< "Cannot Generate PoreNetwork, please check NetworkSpecs.in for errors" <<std::endl;
        return 1;
    }
    
    
    std::cout<< "Generating PoreBodies Nrs" << std::endl;
    P->generate_naive_array();
    
    // --- Generate the Network
    std::cout<< "Generating Network" << std::endl;
    P->generateConnectivity();
    P->generateLocation();
    
    // --- Update input Parser to include the changes per forward direction
    float *C = new float[11];
    for(int i = 0; i < 11; i++){
        C[i] = 0.3000f;
    }
    
    
    eliminateThroats(P, C, 6);
    P->removeFlaggedThroats(-1);
    
    
    size_t a = P->generateFullConnectivity();
  
    char * pb_list = searchForIsolatedPB(P);
     
    
    P->removeFlaggedPBs(pb_list, (char)2);
    
    writeConnectivity(cFile.c_str(),P);
    writeLocation(lFile.c_str(), P);
    
    //if(writeVTKswitch)
    //    writeVTK(vtkFile.c_str(), P);
    

}




