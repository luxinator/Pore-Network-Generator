#include <iostream>
#include <vector>
#include <iomanip>
#include "ArrayFunctions.h"
#include "Generator.h"


int main() {
    
    int Ni = 5;
    int Nj = 5;
    int Nk = 5;
    
    int*** arr = generate_naive_array(Ni,Nj,Nk);
    
    
    std::vector<std::pair<int, int>> *connect = generateConnectivity(Ni,Nj,Nk, arr);
    
    // set precision output

    std::setprecision(8);
    std::cout<< connect->size() << std::endl;
    
    
    for(int PN = 0; PN <= Ni*Nj*Nk; PN++){
    //        std::cout<< connect->at(PN).first << " "<<connect->at(PN).second << std::endl;
    }
    
    location *locations = generateLocation(connect, Ni, Nj, Nk, 1.0f);
   
    
    
     for(int i = 1; i <= Ni*Nj*Nk; i++){
        std::cout << '[' << i << ']'<< ' ';
        std::cout << std::setw(8)<< std::get<0>(locations[i]) << " ";
        std::cout << std::setw(8)<< std::get<1>(locations[i]) << " ";
        std::cout << std::setw(8)<< std::get<2>(locations[i]) << " ";
        std::cout << std::setw(8)<< std::get<3>(locations[i]) << " ";
        std::cout << std::setw(8)<< std::get<4>(locations[i]) << std::endl;
    }
    

}




