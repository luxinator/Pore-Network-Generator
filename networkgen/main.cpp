
#include "ArrayFunctions.h"
#include "Generator.h"
#include "vtk.h"
#include "inputParser.h"
#include "Eliminator.h"


int main() {
    
    std::cout << "Pore Network Generator Compiled at " << __DATE__ << ' ' << __TIME__<<std::endl;
    std::cout << "Copyright Lucas van Oosterhout. All Rights Reserverd. \n\n" << std::endl;
    
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
    float *C = new float[11];
    for(int i = 0; i < 11; i++){
        C[i] = 0.800f;
    }
    
    EliminateThroats(P, C, 6);
    cleanThroatList(P, -1);
    
    //writeConnectivity(cFile.c_str(),P->throatList);
    
    // --- Testing Full connect generator--- \\
    
    int **test = generateFullConnectivity(Ni, Nj, Nk, P->throatList);
    delete [] P->throatList[0];
    P->throatList[0] = test[0];
    P->throatList[1] = test[1];

    char * pb_list = searchIsolated(P);
    
    //for(int i = 0; i < Ni*Nj*Nk; i ++)
    //    std::cout<< (int)pb_list[i] << std::endl;
    writeConnectivity(cFile.c_str(),P->throatList);
    removeIsolatedPBs(P, pb_list, 2);
    writeLocation(lFile.c_str(), P->locationList, P->throatCounter, Ni*Nj*Nk);
    writeConnectivity(dFile.c_str(),P->throatList);
    
    //writeVTK(vtkFile.c_str(), P->throatList,P->locationList, pb_list, Ni, Nj, Nk);
    
    
    

}




