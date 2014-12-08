//
//  Eliminator.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "Eliminator.h"

#include <random>


/*
 * C[0] = x-dir; C[1] = y-dir; C[2] = z-dir
 * C[3] = x - -y diag; 
 * x++ plane
 * C[4] = x - +y diag; C[5] = x - -y; C[6] = x - -z diag
 * C[7] = x - +z diag;
 *
 * Max coordination number is 6 at the moment (in forward x dir!)
 */

void EliminateThroats(PoreNetwork *P_net, float * C, int coordNr){

    if(!P_net){
        std::cerr << "Eliminator Error! \n No PoreNetwork Specified!" << std::endl;
        return;
    }
//    int nrPB = P_net->ns->Ni * P_net->ns->Nj * P_net->ns->Nk;
    int Ni = P_net->ns->Ni;
    int Nj = P_net->ns->Nj;
    int Nk = P_net->ns->Nk;
    
    //Set up Random Generator in three Directions
    std::random_device r_dev{};
    std::default_random_engine e{r_dev()};
    std::uniform_real_distribution<float> d{0.0f,1.0f};
    
    int *coord   = new int[3];
    int *coord_n = new int[3];

    int pn;
    
    // Go through throatList
    for(int i = 0; P_net->throatList[0][i] != 0; i++){
    
        //Get coordnaites for pb
        deflatten_3d(P_net->throatList[0][i], Ni, Nj, Nk, coord);
        pn = P_net->throatList[0][i];
        
        while (P_net->throatList[0][i] == pn) {
            // check for this pore, all its connections
            deflatten_3d(P_net->throatList[1][i], Ni, Nj, Nk, coord_n);

            // ------
            // We have a connection in the x-dir
            if(d(e) >= C[0] && coord[2] == coord_n[2] && coord[1] == coord_n[1]){
                P_net->throatList[0][i] = -1; // from pb
                P_net->throatList[1][i] = -1; // to pb
                }
            
            // Connection in y-dir
            else if(d(e) >= C[1] && coord[0] == coord_n[0] && coord[2] == coord_n[2]){
                P_net->throatList[0][i] = -1; // from pb
                P_net->throatList[1][i] = -1; // to pb
            }
            // Connection in z-dir
            else if(d(e) >= C[2] && coord[0] == coord_n[0] && coord[1] == coord_n[1]){
                P_net->throatList[0][i] = -1; // from pb
                P_net->throatList[1][i] = -1; // to pb
            }
            

            //------
            else if(coordNr > 3){
                // Connection in x - y diagonal
                if(d(e) >= C[3] && coord[0] != coord_n[0] && coord[1] != coord_n[1] && coord[2] == coord_n[2]){
                    P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                }
                // x++ plane connection
                // Connection in x - -y diagonal
                else if(d(e) >= C[4] && coord[0] != coord_n[0]
                   && coord[1] - coord_n[1] == 1
                   && coord[2] == coord_n[2]){
                    P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pbt
                }
                
                // Connection in x - +y diagonal
                else if(d(e) >= C[5] && coord[0] != coord_n[0]
                   && coord[1]  - coord_n[2] == -1
                   && coord[2] == coord_n[1]){
                    P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                }
                
                // Connection in x - +z diagonal
                else if(d(e) >= C[6] && coord[0] != coord_n[0]
                   && coord[1] == coord_n[1]
                   && coord[2] - coord_n[2] == -1){
                    P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                }
                
                // Connection in x - -z diagonal
                else if(d(e) >= C[7] && coord[0] != coord_n[0]
                        && coord[1] == coord_n[1]
                        && coord[2] - coord_n[2] == 1){
                    P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                }
                
            }// Coord If
            
            i++;
        }// while
    }// for
    
    
}



/*
 * Search for isolated PoreBodies and update PB list accordingly
 */

void searchIsolated(PoreNetwork *P_net){
    
    int Ni = P_net->ns->Ni;
    int Nj = P_net->ns->Nj;
    int Nk = P_net->ns->Nk;
    
    // Allocata a chunk and set it to zero
    char *t = new char[Ni*Nj*Nk];
    memset(t, 0, Ni*Nj*Nk);
    
    char ***flag = new char**[Ni];
    
    int i,j;
    for(i = 0; i < Ni; i++){
        flag[i] = new char*[Nj];
        for(j = 0; j < Nj; j++)
            flag[i][j] = t + (i * Nj * Nk) + (j * Nk);
    }
    
    int *coord = new int[3];
    
    // for x-plane flag all connected pbs -> x-plane +1 for all flagged pbs
    for(i = 0; i < Ni; i++){
        
    }
    
    
    
}