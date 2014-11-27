#include <iostream>
#include <vector>
#include <iomanip>
#include "ArrayFunctions.h"
#include "Generator.h"


int main() {
    
    int Ni = 50;
    int Nj = 50;
    int Nk = 50;
    
    int*** arr = generate_naive_array(Ni,Nj,Nk);
    
    int **throatCounter = new int*[2];
    throatCounter[0] = new int[Ni*Nj*Nk];
    throatCounter[1] = new int[Ni*Nj*Nk];

    std::vector<std::pair<int, int>> *connect = generateConnectivity(Ni, Nj, Nk, arr, throatCounter);
    
    // set precision output

    std::setprecision(8);
    //std::cout<< connect->size() << std::endl;
    
    //for(int PN = 0; PN < connect->size(); PN++){
    //        std::cout<<connect->at(PN).first << " "<<connect->at(PN).second << std::endl;
    //}
    
    generateLocation(0.25e-4, throatCounter, Ni, Nj, Nk);
    
}




