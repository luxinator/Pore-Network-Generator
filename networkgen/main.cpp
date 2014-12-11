
#include "ArrayFunctions.h"
#include "Generator.h"
#include "vtk.h"
#include "inputParser.h"
#include "Eliminator.h"


int main() {
    
    NetworkSpecs *ns = readSpecsFile("/Users/lucas/Programming/Xcode/PoreNetworkgen/data/NetworkSpecs.in");
    
    PoreNetwork *P = new PoreNetwork;
    P->ns = ns;
    
    
    // make it so that Ni = Ni + 2, for the boundaries
    int Ni = ns->Ni;
    int Nj = ns->Nj;
    int Nk = ns->Nk;
    float L = ns->Length;
    
    std::string cFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/connectivity.txt";
    std::string dFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/deleted.txt";
    std::string vtkFile = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/data.vtk";
    std::string lFile   = "/Users/lucas/Programming/Xcode/PoreNetworkgen/data/location.txt";
    
    std::cout<< "Generating PoreBodies Nrs" << std::endl;
    P->arr = generate_naive_array(Ni,Nj,Nk);
    
    /*
     * This should most defentily go somewhere else!
     */
    //Allocate a Part of Memory 
    int *t = new int[2 * Ni*Nj*Nk];
    P->throatCounter = new int*[2];
    //Point the two pointers to their places in the Large Part of memory
    P->throatCounter[0] = t;
    P->throatCounter[1] = t + (Ni*Nj*Nk);
    
    
    // --- Generate the Network
    std::cout<< "Generating Network" << std::endl;
    P->throatList = generateConnectivity(Ni, Nj, Nk, P->arr, P->throatCounter);
    P->locationList = generateLocation(L, P->throatCounter, Ni, Nj, Nk);
    
    // --- Update input Parser to include the changes per forward direction
    float *C = new float[8];
    for(int i = 0; i < 8; i++){
        C[i] = 0.0002;
    }
    
    EliminateThroats(P, C, 6);
    cleanThroatList(P, -1);
    
    
    // --- Testing Full connect generator--- \\
    
    int **test = generateFullConnectivity(Ni, Nj, Nk, P->throatList);
    delete [] P->throatList[0];
    P->throatList[0] = test[0];
    P->throatList[1] = test[1];
    
    //writeConnectivity(cFile.c_str(),test);
    //writeLocation(lFile.c_str(), P->locationList, P->throatCounter, Ni*Nj*Nk);
   
    
    char * pb_list = searchIsolated(P);
    
    writeVTK(vtkFile.c_str(), P->throatList,P->locationList,pb_list, Ni, Nj, Nk);
    //removeIsolatedPBs(P, pb_list, 1);
    

}




