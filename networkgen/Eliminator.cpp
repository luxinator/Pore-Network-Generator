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
    
    // Go through throatList
    for(i = 0; P_net->throatList[0][i] != 0; i++){
    
        //Get coordnaites for pb
        deflatten_3d(P_net->throatList[0][i], Ni, Nj, Nk, coord);
        pn = P_net->throatList[0][i];
        
        while (P_net->throatList[0][i] == pn) {
            // check for this pore, all its connections
            deflatten_3d(P_net->throatList[1][i], Ni, Nj, Nk, coord_n);

            // ------
            // We have a connection in the x-dir
            if(d(e) >= ChanceList[0] && coord[2] == coord_n[2] && coord[1] == coord_n[1]){
                //P_net->throatList[0][i] = -1; // from pb
                P_net->throatList[1][i] = -1; // to pb
                deleted++;
                }
            
            // Connection in y-dir
            else if(d(e) >= ChanceList[1] && coord[0] == coord_n[0] && coord[2] == coord_n[2]){
                //P_net->throatList[0][i] = -1; // from pb
                P_net->throatList[1][i] = -1; // to pb
                deleted++;
            }
            // Connection in z-dir
            else if(d(e) >= ChanceList[2] && coord[0] == coord_n[0] && coord[1] == coord_n[1]){
                //P_net->throatList[0][i] = -1; // from pb
                P_net->throatList[1][i] = -1; // to pb
                deleted++;
            }
            

            //------
            else if(coordNr > 3){
                // Connection in x - y diagonal
                if(d(e) >= ChanceList[3] && coord[0] != coord_n[0] && coord[1] != coord_n[1] && coord[2] == coord_n[2]){
                    //P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                    deleted++;
                }
                // x++ plane connection
                // Connection in x - -y diagonal
                else if(d(e) >= ChanceList[4] && coord[0] != coord_n[0]
                   && coord[1] - coord_n[1] == 1
                   && coord[2] == coord_n[2]){
                    //P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pbt
                    deleted++;
                }
                
                // Connection in x - +y diagonal
                else if(d(e) >= ChanceList[5] && coord[0] != coord_n[0]
                   && coord[1]  - coord_n[2] == -1
                   && coord[2] == coord_n[1]){
                   // P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                    deleted++;
                }
                
                // Connection in x - +z diagonal
                else if(d(e) >= ChanceList[6] && coord[0] != coord_n[0]
                   && coord[1] == coord_n[1]
                   && coord[2] - coord_n[2] == -1){
                    //P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                    deleted++;
                }
                
                // Connection in x - -z diagonal
                else if(d(e) >= ChanceList[7] && coord[0] != coord_n[0]
                        && coord[1] == coord_n[1]
                        && coord[2] - coord_n[2] == 1){
                   // P_net->throatList[0][i] = -1; // from pb
                    P_net->throatList[1][i] = -1; // to pb
                    deleted++;
                }
                
            }// Coord If
            
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
    while( throatList[0][i] == pn && i < max){
        i++;
    }
    
    return (i-1);
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
 */

void DFS(int start, int ** TL, char* flagged_PB, int TL_Length){
    // Flag PB as discoverd
    flagged_PB[TL[0][start]] = 1;
    
    int max = returnAdjecentNodes(TL, start, TL_Length);
    
    // We are on a leaf
    if( max == start)
        return;
    
    //For all nodes do:
    else{
        for(int i = start + 1; i <= max; i++){
            if(flagged_PB[TL[1][i]] == 0)
                DFS(i, TL, flagged_PB, TL_Length);
            // else flagged_PB == 1 -> Already Did this part of the graph
        }
    }
}

/*
 * Search for isolated PoreBodies and update PB list accordingly
 * Search the FULL connectivity map foward and backward, checking
 * if pbs are connected to inlet AND outlet
 */

void searchIsolated(PoreNetwork *P_net){
    
    
    std::cout<< "Starting Search for Isolated PBs and Clusters" << std::endl;
    int Ni = P_net->ns->Ni;
    int Nj = P_net->ns->Nj;
    int Nk = P_net->ns->Nk;
    
    int i;
    for(i = 0; P_net->throatList[0][i] != 0; i++)
        ;
    int lengthTL = i;
    
    // Allocata a chunk and set it to zero
    char *flagged_PB = new char[Ni*Nj*Nk];
    
    for(i = 0; i < Ni*Nj*Nk; i++)
        flagged_PB[i] = 0;
    
    //Do a DepthFirst Search on all inlets
    for(i = 0; i < Nj*Nk; i++){
        // First test if Inlet has connection
        if(P_net->throatList[0][i] > 0  && P_net->throatList[0][i] < Nj*Nk){
            DFS(i, P_net->throatList, flagged_PB, lengthTL - 1); // same as with sorting, do not allow guards to be searched!
        }
    }
    
    // we now have a flagged list of pb's which are connected to the inlets
    for(i = 0; i < Ni*Nj*Nk; i++){
        //if (flagged_PB[i]== 1) {
            std::cout<<'[' << i << ']'<< '\t' << (int)flagged_PB[i] << std::endl;
        //}
        
    }
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









