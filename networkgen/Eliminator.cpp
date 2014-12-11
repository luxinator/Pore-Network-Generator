//
//  Eliminator.cpp
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#include "Eliminator.h"
#include <random>


inline void delelteThroat(PoreNetwork *P_net, int pn, int i, int deleted){
    
    P_net->throatList[1][i] = -1;
    //P_net->throatCounter[0][pn] -= 1;
    //P_net->throatCounter[1][pn] -= deleted;
    deleted++;
}

/*
 * C[0] = x-dir; C[1] = y-dir; C[2] = z-dir
 * C[3] = x - -y; C[4] = x - +y;
 * C[5] = x - -z; C[6] = x - +z;
 * C[7] = y - -z; C[8] = y - +z;
 * C[9] = -y - -z;C[10]= -y - + z;
 *
 * Max coordination number is 6 at the moment (in forward x dir!)
 *
 */

void EliminateThroats(PoreNetwork *P_net, float * ChanceList, int coordNr){
    
    
    std::cout<<"Deleting throats" << std::endl;
    
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
    int deleted = 0;
    int pn, i;
    
    i = 0;
    // Go through throatList
    while(P_net->throatList[0][i] != 0){
        
        //Get coordnaites for pb
        deflatten_3d(P_net->throatList[0][i], Ni, Nj, Nk, coord);
        
        pn = P_net->throatList[0][i];
        
        while (P_net->throatList[0][i] == pn) {
            // check for this pore, all its connections
            deflatten_3d(P_net->throatList[1][i], Ni, Nj, Nk, coord_n);
            
            // We have a connection in the x-dir
            if(d(e) >= ChanceList[0]
               && coord[0] - coord_n[0] == -1
               && coord[1] - coord_n[1] == 0
               && coord[2] - coord_n[2] == 0){
                delelteThroat(P_net, pn, i, deleted);
                
            }
            
            // Connection in y-dir
            else if(d(e) >= ChanceList[1]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 0){
                delelteThroat(P_net, pn, i, deleted);
            }
            // Connection in z-dir
            else if(d(e) >= ChanceList[2]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == -1){
                delelteThroat(P_net, pn, i, deleted);
            }
            
            
            //------
            // Connection in x - -y diagonal
            else if(d(e) >= ChanceList[3]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 1
                    && coord[2] - coord_n[2] == 0){
                delelteThroat(P_net, pn, i, deleted);
            }
            // Connection in x - +y diagonal
            else if(d(e) >= ChanceList[4]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 0){
                delelteThroat(P_net, pn, i, deleted);
            }
            
            // Connection in x - -z diagonal
            else if(d(e) >= ChanceList[5]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == 1){
                delelteThroat(P_net, pn, i, deleted);
            }
            
            // Connection in x - +z diagonal
            else if(d(e) >= ChanceList[6]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == -1){
                delelteThroat(P_net, pn, i, deleted);
            }
            
            // Connection in y - -z diagonal
            else if(d(e) >= ChanceList[7]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == 1
                    && coord[2] - coord_n[2] == 1){
                delelteThroat(P_net, pn, i, deleted);
            }
            
            // Connection in y - +z diagonal
            else if(d(e) >= ChanceList[8]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == 1
                    && coord[2] - coord_n[2] == -1){
                delelteThroat(P_net, pn, i, deleted);
            }
            // Connection in -y - -z diagonal
            else if(d(e) >= ChanceList[9]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 1){
                delelteThroat(P_net, pn, i, deleted);
            }
            // Connection in -y - +z diagonal
            else if(d(e) >= ChanceList[10]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == -1){
                delelteThroat(P_net, pn, i, deleted);
            }
            
            
            i++;
        }// while
        
    }// for
    delete [] coord;
    delete [] coord_n;
    
    
}


/*
 * Returns the index of the last of the connections to pb i
 * And thus with index i, returns all connection to pb
 * Maybe INLINE?
 */

int returnAdjecentNodes(int **throatList, int i, int max){
    
    int pn = throatList[0][i];
    int pn_n;
    int pn_1;
    while( throatList[0][i] == pn && i < max){
        pn_1 = throatList[0][i];
        pn_n = throatList[1][i];
        i++;
    }
    
    return (i);
}

/*
 1  procedure DFS(G,v):
 2      label v as discovered
 3      for all edges from v to w in G.adjacentEdges(v) do
 4          if vertex w is not labeled as discovered then
 5              recursively call DFS(G,w)
 */

/*
 * Recursive Depth-first Search
 * Places a flag in the flagged_pb array, when check is found!
 */

void DFS(int start, int ** TL, char* flagged_PB, int TL_Length, char flag, char check){
    // Flag PB as discoverd
    flagged_PB[TL[0][start]] = flag;
    
    int max = returnAdjecentNodes(TL, start, TL_Length);
    // from TL[1][start] to TL[1][max] are connected pbs
    
    //For all throats connected to pb do:
    
    for(int i = start; i < max; i++){
        if(flagged_PB[TL[1][i]] == check){
            //search for edges from TL[1][j]
            for(int j = 0; j < TL_Length; j++)
            {
                //connection to pb
                if(TL[0][j] == TL[1][i]){
                    DFS(j, TL, flagged_PB, TL_Length, flag, check);
                }
            }
        }
    }
}


/*
 * Search for isolated PoreBodies and update PB list accordingly
 * Search the FULL connectivity map foward and backward, checking
 * if pbs are connected to inlet AND outlet
 */

char * searchIsolated(PoreNetwork *P_net){
    
    
    std::cout<< "Starting Search for Isolated PBs and Clusters" << std::endl;
    int Ni = P_net->ns->Ni;
    int Nj = P_net->ns->Nj;
    int Nk = P_net->ns->Nk;
    
    int i;
    for(i = 0; P_net->throatList[0][i] != 0; i++){
        //std::cout << P_net->throatList[0][i] << '\t' << P_net->throatList[1][i] << std::endl;
    }
    
    int lengthTL = i;
    
    // Allocata a chunk and set it to zero
    char *flagged_PB = new char[Ni*Nj*Nk];
    for(i = 0; i < Ni*Nj*Nk; i++){
        flagged_PB[i] = 0;
    }
    //Do a DepthFirst Search on all inlets
    for(i = 0; P_net->throatList[0][i] < Nj*Nk; i++){
        DFS(i, P_net->throatList, flagged_PB, lengthTL - 1, (char)1, (char) 0); // same as with sorting, do not allow guards to be searched
    }
    //Do a Depth First Search on all outlets
    for(i = lengthTL -1 ; P_net->throatList[0][i] > (Ni*Nj*Nk - Nj*Nk); i--){
        DFS(i, P_net->throatList, flagged_PB, lengthTL - 1, (char)2, (char) 1);
    }
    
    
    // we now have a flagged list of pb's which are connected to the inlets
    return flagged_PB;
}



/*
 1  procedure BFS(G,v) is
 2      create a queue Q
 3      create a set V
 4      add v to V
 5      enqueue v onto Q
 6      while Q is not empty loop
 7         t ← Q.dequeue()
 8         if t is what we are looking for then
 9            return t
 10        end if
 11        for all edges e in G.adjacentEdges(t) loop
 12           u ← G.adjacentVertex(t,e)
 13           if u is not in V then
 14               add u to V
 15               enqueue u onto Q
 16           end if
 17        end loop
 18     end loop
 19     return none
 20 end BFS
 */









