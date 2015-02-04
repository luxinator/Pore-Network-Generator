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
 * C[3] = x - -y; C[4] = x - +y;
 * C[5] = x - -z; C[6] = x - +z;
 * C[7] = y - -z; C[8] = y - +z;
 * C[9] = -y - -z;C[10]= -y - +
 *
 * Max coordination number is 12 at the moment (in forward x dir!)
 *
 */

void eliminateThroats(PoreNetwork *P_net, int coordNr){
   
     
    std::cout<<"Deleting throats" << std::endl;

    float * ChanceList = P_net->ns->C;
    
    if(!P_net){
        std::cerr << "Eliminator Error! \n\t No PoreNetwork Specified!" << std::endl;
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
    size_t deleted = 0;
    size_t pn = 1, i = 0;
    
    float xChance = ChanceList[0];
    float yChance = ChanceList[1];
    float zChance = ChanceList[2];
    
    // Go through throatList
    while(P_net->throatList[0][i] != 0){
        
        //Get coordnaites for pb
        deflatten_3d(P_net->throatList[0][i], Ni, Nj, Nk, coord);
        
        pn = P_net->throatList[0][i];
        /*if(pn < Nj*Nk || pn > Ni*Nj*Nk - Nj*Nk){
            i++;
            continue;
        }
        */
        // Inlet and outlet pores are skipped
        if(P_net->ns->flowDirs[0] && coord[0] == 0 ){
            i++;
            continue;
        }
        if(P_net->ns->flowDirs[1] && coord[1] == 0 ){
            i++;
            continue;
        }
        if(P_net->ns->flowDirs[2] && coord[2] == 0 ){
            i++;
            continue;
        }
        if(P_net->ns->flowDirs[0] && coord[0] ==  Ni -2 ){
            i++;
            ChanceList[0] = 1.0f; // X-dir is kept, else may fail -> simple and ugly hack
        }
        if(P_net->ns->flowDirs[1] && coord[1] ==  Nj - 2 ){
            i++;
            ChanceList[1] = 1.0f; // Y-dir is kept, else may fail -> simple and ugly hack
        }
        if(P_net->ns->flowDirs[2] && coord[2] ==  Nk - 2 ){
            i++;
            ChanceList[2] = 1.0f; // Z-dir is kept, else may fail -> simple and ugly hack
        }
        
        while (P_net->throatList[0][i] == (int)pn) {
            // check for this pore, all its connections
            deflatten_3d(P_net->throatList[1][i], Ni, Nj, Nk, coord_n);
            
            
            // We have a connection in the x-dir
            if(d(e) >= ChanceList[0]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            // Connection in y-dir
            else if(d(e) >= ChanceList[1]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in z-dir
            else if(d(e) >= ChanceList[2]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == -1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            //------
            // Connection in x - -y diagonal
            else if(d(e) >= ChanceList[3]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 1
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in x - +y diagonal
            else if(d(e) >= ChanceList[4]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            // Connection in x - -z diagonal
            else if(d(e) >= ChanceList[5]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == 1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            // Connection in x - +z diagonal
            else if(d(e) >= ChanceList[6]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == -1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            // Connection in y - -z diagonal
            else if(d(e) >= ChanceList[7]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == 1
                    && coord[2] - coord_n[2] == 1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            // Connection in y - +z diagonal
            else if(d(e) >= ChanceList[8]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == 1
                    && coord[2] - coord_n[2] == -1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in -y - -z diagonal
            else if(d(e) >= ChanceList[9]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in -y - +z diagonal
            else if(d(e) >= ChanceList[10]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == -1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            i++;
        }// while
        
        
        //P_net->throatCounter[1][pn] -= deleted; //Not really nice, Only the PoreNetwork Class should change the PoreNetwork...
    }// while
    
    //Hackish but it works
    ChanceList[0] = xChance;
    ChanceList[1] = yChance;
    ChanceList[2] = zChance;
    
    if(P_net->ns->periodicBounndaries){
        size_t pn_n;
        //Periodic throats can be eliminated as well!
        for(size_t perdiocC = 0; P_net->periodicThroats[perdiocC] != 0; perdiocC++){
            pn   = P_net->throatList[0][P_net->periodicThroats[perdiocC]];
            pn_n = P_net->throatList[1][P_net->periodicThroats[perdiocC]];
            
            deflatten_3d(pn, Ni, Nj, Nk, coord);
            deflatten_3d(pn_n, Ni, Nj, Nk, coord_n);
            
            
            // Delete in y-dir
            if( d(e) >= ChanceList[1] &&
               coord[1] == coord_n[1] &&
               coord[2] != coord_n[2]){
                deleted = P_net->delelteThroat((size_t)P_net->periodicThroats[perdiocC], deleted, -1);
                //std::cout << P_net->periodicThroats[perdiocC] << '\t' << pn << " - " << pn_n << std::endl;
                
            }
            if(d(e) >= ChanceList[2] &&
               coord[1] != coord_n[1] &&
               coord[2] == coord_n[2]){
                deleted = P_net->delelteThroat((size_t)P_net->periodicThroats[perdiocC], deleted, -1);
            }
        } //for periodicThroat
    } // if periodicBoundaries
    
    delete [] coord;
    delete [] coord_n;
    
    
}


/*
 * Returns the index of the last of the connections to pb i
 * And thus with index i, returns all connection to pb
 * Maybe INLINE?
 */

size_t returnAdjecentNodes(int **throatList, size_t i, size_t max){
    
    int pn = throatList[0][i];
    while( throatList[0][i] == pn && i < max){
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
 * Recursive Depth-first Search on FullConnected ThroatList
 * Places a flag in the flagged_pb array, when check is found!
 */

void DFS(size_t start, int ** TL, char* flagged_PB, size_t TL_Length, char flag, char check){
 
    // Check if the pb has been visited
    if(flagged_PB[TL[0][start]] == flag)
        return;

    // Flag PB as discoverd and check all adjecent nodes
    flagged_PB[TL[0][start]] = flag;

    // from TL[1][start] to TL[1][max] are connected pbs
    size_t max = returnAdjecentNodes(TL, start, TL_Length);
    
    
    //For all throats connected to pb do:
    for(size_t i = start; i < max; i++){
        if(flagged_PB[TL[1][i]] == check){
            //search edges of this node
            for(size_t j = 0; j < TL_Length; j++)
            {// do DFS on all edges of this node
                if(TL[0][j] == TL[1][i]){
                    DFS(j, TL, flagged_PB, TL_Length, flag, check);
                }
            }
        }
    }
}


/*
 * Search for isolated PoreBodies and return a Flagged Pore Bodies List
 * Search the FULL connectivity map foward and backward, checking
 * if pbs are connected to inlet AND outlet
 *
 * Call removeFlaggedPBs when this has been run, the network is invalid!
 */


char * searchForIsolatedPB(PoreNetwork *P_net){
    
    bool verbose = false;
    
    
    std::cout<< "Starting Search for Isolated PBs and Clusters" << std::endl;
    int Ni = P_net->ns->Ni;
    int Nj = P_net->ns->Nj;
    int Nk = P_net->ns->Nk;
    
    size_t i;
    for(i = 0; P_net->throatList_full[0][i] != 0; i++){
        ;
    }
    
    size_t lengthTL = i;
    
    //std::cout<< "TL: " <<lengthTL << std::endl;
    
    // Allocata a chunk of mem and set it to zero
    char *flagged_PB = new char[Ni*Nj*Nk+1];
    for(i = 0; (int)i <= Ni*Nj*Nk; i++){
        flagged_PB[i] = 0;
    }
    
    //Do a DepthFirst Search on all inlets
    for(i = 0; P_net->throatList_full[0][i] <= Nj*Nk; i++){
        DFS(i, P_net->throatList_full, flagged_PB, lengthTL, (char)1, (char)0);
        //std::cout << i << '\t' << P_net->throatList_full[0][i] << '\t'<< (int)flagged_PB[ P_net->throatList_full[0][i]] << std::endl;
        // same as with sorting, do not allow guards to be searched
    }
    
    if(verbose)
        for(int i = 1; i <= Ni*Nj*Nk; i ++){
        std::cout << "PB: " <<i << "\t Flag: " <<(int)flagged_PB[i] << std::endl;
        }
    
    bool brokenNetwork = true;
    
    //Do a Depth First Search on all outlets

    for(i = lengthTL - 1 ; P_net->throatList_full[0][i] >= (Ni*Nj*Nk - Nj*Nk); i--){
        
        // Check if pb the qualifies
        if(flagged_PB[ P_net->throatList_full[0][i]] == (char)1){
            
            DFS(i, P_net->throatList_full, flagged_PB, lengthTL, (char)2, (char)1);
            //std::cout << i << '\t' << P_net->throatList_full[0][i] << '\t'<< (int)flagged_PB[ P_net->throatList_full[0][i]] << std::endl;
            brokenNetwork = false;
        }
    }
    if (brokenNetwork){
        std::cout << "!!! --- 2 : Network is Broken no Connection between Inlet and Outlet ---!!!" << std::endl;
        return nullptr;
    }
    
    // we now have a flagged list of pb's which are connected to the inlets AND outlets
    std::cout << "Done!"<< std::endl;
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









