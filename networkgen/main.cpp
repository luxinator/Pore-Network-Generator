
#include "ArrayFunctions.h"
#include "vtk.h"
#include "inputParser.h"
#include "Eliminator.h"


int main() {
    
    std::cout << "\nPore Network Generator Compiled at " << __DATE__ << ' ' << __TIME__<<std::endl;
    std::cout << "Copyright Lucas van Oosterhout. All Rights Reserverd. \n\n" << std::endl;
    
    std::string nSpecs = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/NetworkSpecs.in";
    
    std::string cFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/connectivity.txt";
    std::string dFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/deleted.txt";
    std::string vtkFile = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/data.vtk";
    std::string lFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/location.txt";
    
    
    
    PoreNetwork *P = new PoreNetwork(nSpecs.c_str());
    
    std::cout<< "Generating PoreBodies Nrs" << std::endl;
    P->generate_naive_array();
    
    // --- Generate the Network
    std::cout<< "Generating Network" << std::endl;
    P->generateConnectivity();
    P->generateLocation();
    
    // --- Update input Parser to include the changes per forward direction
    float *C = new float[11];
    for(int i = 0; i < 11; i++){
        C[i] = 0.6000f;
    }
    
    
    eliminateThroats(P, C, 6);
    P->removeFlaggedThroats(-1);
    
   // writeConnectivity(cFile.c_str(),P->throatList);
    writeLocation(dFile.c_str(), P);
    
    // --- Testing Full connect generator---
    P->generateFullConnectivity();
  
    
    // !--- keep hold of the half connectivity map! need it for the model runs with dynamic pressure!

    char * pb_list = searchForIsolatedPB(P);
    
    //for(int i = 0; i < Ni*Nj*Nk; i ++)
    //    std::cout<< (int)pb_list[i] << std::endl;
    writeConnectivity(cFile.c_str(),P);
    P->removeFlaggedPBs(pb_list, 2);
    
    writeLocation(lFile.c_str(), P);
    //writeConnectivity(dFile.c_str(),P->throatList);
    
    writeVTK(vtkFile.c_str(), P);
    

}




