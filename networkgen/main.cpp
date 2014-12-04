
#include "ArrayFunctions.h"
#include "Generator.h"
#include "vtk.h"
#include "inputParser.h"


int main() {
    
    NetworkSpecs *ns = readSpecsFile("/Users/lucas/Programming/Xcode/PoreNetworkgen/data/NetworkSpecs.in");
    
    
    /*
    // make it so that Ni = Ni + 2, for the boundaries
    int Ni = 20;
    int Nj = 50;
    int Nk = 50;
    
    std::string cFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/connectivity.txt";
    std::string vtkFile = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/data.vtk";
    std::string lFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/location.txt";
    
    int*** arr = generate_naive_array(Ni,Nj,Nk);
    
    //Allocate a Larte Part of Memory
    int *t = new int[2 * Ni*Nj*Nk];
    
    //Make a pointer to a list of 2 pointers to ints
    int **throatCounter = new int*[2];
    //Point the two pointers to their places in the Large Part of memory
    throatCounter[0] = t;
    throatCounter[1] = t + (Ni*Nj*Nk);
    
    // Generate the Network
    int ** connect = generateConnectivity(Ni, Nj, Nk, arr, throatCounter);
    float** locationList = generateLocation(1.0f, throatCounter, Ni, Nj, Nk);
    
    writeConnectivity(cFile.c_str(), connect, Ni*Nj*Nk);
    
    writeLocation(lFile.c_str(), locationList, throatCounter, Ni*Nj*Nk);
    writeVTK(vtkFile.c_str(), connect, locationList, Ni, Nj, Nk);
    
    */
}




