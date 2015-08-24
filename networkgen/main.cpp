/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
 

#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>

#include "ArrayFunctions.h"
#include "vtk.h"
#include "inputParser.h"
#include "Eliminator.h"
#include "writerFunctions.h"
#include "Combinator.h"

// Forward Declaration
int Finalize(PoreNetwork * pn);

std::string nSpecs = "./data/NetworkSpecs.in";
std::string cFile   = "./data/";
std::string fcFile   = "./data/";
std::string lFile   = "./data/";

std::string vtkFile = "./data/";

int main(int argc, char *argv[]) {
    std::cout << "\nPore Network Generator Compiled at " << __DATE__ << ' ' << __TIME__<<std::endl;
    std::cout << "Copyright Lucas van Oosterhout. GPLv2 see LICENSE " << std::endl;
    std::cout << "Contact : l.m.vanoosterhout@uu.nl\n\n" << std::endl;

    const int min_int = std::numeric_limits<int>::min();
    const int max_int = std::numeric_limits<int>::max();
    std::cout << "Please be ABOSLUTELY SURE that no more then " << max_int << " Pore bodies\nare present in the System"  << std::endl;

    std::string helpText = "Options can be combined for different results.\n \
    Command-Line Options:\n \
    -h               \t Shows this help text\n \
    -ns [location]   \t Specify the location of the NetworkSpecs.in file if not given standard location is used [../data/NetworkSpecs.in]\n \
    -cfile [location] \t Specify the location of the connectiviy output file\n \
    -fcfile [location]\t Specify the location of the fullconnectivity output file\n \
    -lfile [location] \t Specify the location of the locations file\n \
    -vtk [location]   \t Specify if a vtk file is to be written and where\n \
    -inner [location] \t Load Inner Network from file(s) and Generate with Boundaries as specified in the NetworkSpecs.in\n \
    -combine [top_network] [bottom_network] [Separation Dist] [Search Dist] [Survival] combines (innner) networks into one network with boundaries as specified in bot_network/NetworkSpecs.in \n";
    
    nSpecs = "./data/NetworkSpecs.in";
    cFile   = "./data/";
    fcFile   = "./data/";
    lFile   = "./data/";
    
    vtkFile = "./data/";
 
    std::string name = "";
    std::string top_network, bot_network = "";
    
    bool inputWasParsed = false;
    bool writeVTKswitch = false;
    bool generate = true;
    bool combine = false;

	
	float c_sep_dist 	= 0.0f;
	float c_search_dist = 0.0f;
	float c_survival 	= 0.0f;
    

    // Go through the list of given args
    if (argc > 1) {
        for(int i = 1; i < argc; i++){
            std::string s = std::string(argv[i]);
            
            if(s.compare(0,2,"-h") == 0){
                std::cout<< helpText << std::endl;
                return 0;
            }
            else if(s.compare(0,3, "-ns") == 0){
                inputWasParsed = true;
                nSpecs = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,6, "-cfile") == 0){
                inputWasParsed = true;
                cFile = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,7, "-fcfile") == 0){
                inputWasParsed = true;
                fcFile = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,4, "-vtk") == 0){
                inputWasParsed = true;
                vtkFile = std::string(argv[i+1]);
                writeVTKswitch = true;
                i++;
            }
            else if(s.compare(0,6, "-lfile") == 0){
                inputWasParsed = true;
                lFile = std::string(argv[i+1]);
                i++;
            }
            else if(s.compare(0,6, "-inner") == 0){
                inputWasParsed = true;
                generate = false;
                name = std::string(argv[i+1]);
		
                i++;
            }
            else if(s.compare(0,8, "-combine") == 0 ){
            	inputWasParsed = true;
            	generate = false;
            	combine   = true;
            	try{
            		top_network 	= std::string(argv[i+1]);
            		bot_network 	= std::string(argv[i+2]);
					c_sep_dist 		= std::atof(argv[i+3]);
					c_search_dist 	= std::atof(argv[i+4]);
					c_survival		= std::atof(argv[i+5]);
            	}
            	catch ( ... ){
            		std::cerr << "Error: Not Enough Parameters, see -h for details" << std::endl;
					std::cerr << argv[i+3] << std::endl;
            		return 1;
            	}
				break;
            }
			
			else
                inputWasParsed = false;
        }
        if(!inputWasParsed){
            std::cout << "Error Parsing Input, run with -h for help" << std::endl;
            return 1;
        }
    } else {
        std::cout << "No Arguments given, assuming default behavior" << std::endl;
    }
    
    if(generate){
        PoreNetwork *innerNetwork = new PoreNetwork(nSpecs.c_str());
        if(!innerNetwork->ns){
            std::cout<< "Cannot Generate PoreNetwork, please check NetworkSpecs.in for errors" <<std::endl;
            return 1;
        }
        
        std::cout<< "Generating PoreBodies Nrs" << std::endl;
        innerNetwork->generate_naive_array();
        
        
        // --- Generate the innerNetwork
        std::cout<< "\nGenerating Network" << std::endl;
        innerNetwork->generateConnectivity();
        innerNetwork->generateLocation();
		innerNetwork->generatePbSizes();
        
        std::cout << "Eliminating Throats ..." << std::endl;
        eliminateThroats(innerNetwork);
        innerNetwork->removeFlaggedThroats(-1);
        
        // --- Write the Inner network to file, no search for isolated pbs!

        vtkFile = cFile + innerNetwork->ns->name + ".vtk";
        writeVTK(vtkFile, innerNetwork, innerNetwork->pb_sizeList);
        writeConnectivity(cFile.c_str(), innerNetwork);
        
        writeLocation(lFile.c_str(), innerNetwork);
        writeNetworkSpecs(cFile.c_str(), innerNetwork);


		// --- Generate the Boundaries and Search for Isolated pbds
        std::cout << std::endl;

        if(Finalize(innerNetwork) != 0){
            std::cerr << "Something bad happend. \nI am sorry but, I QUIT!";
            return -1;
        }
//
        delete innerNetwork;
    }
    
    else if(combine){
    	std::cout << "Combining Networks " << top_network << " and " << bot_network << std::endl;

    	PoreNetwork *top = new PoreNetwork(top_network);
    	PoreNetwork *bot = new PoreNetwork(bot_network);
			
    	Combinator *combi = new Combinator(top, bot, "combi");
    	// Gather this from an options file or from input
    	combi->setSeparation(c_sep_dist);
    	combi->setSearchDist(c_search_dist);
    	combi->setSurvival(c_survival);
        combi->sortConnectionsList(false);
    	combi->Combine(0);
    	combi->builtConnectionList();
    	PoreNetwork *Res = combi->getResult(); // This is not a completely valid network! NetworkSpecs is mostly empty!
		
	// **** COMBINED networks are special and don't use Finalize

		// Generate Boudnaries
		Res->generateBoundary(0, bot->ns->meanPBsize,top->ns->meanPBsize, bot->ns->pbDist, top->ns->pbDist);

		size_t lengthTL = Res->generateFullConnectivity();

		char * pb_list = searchForIsolatedPB(Res,lengthTL);
		if(!pb_list){
		std::cout << "Network is Broken Aborting" << std::endl;
			return 1;
		}
		Res->removeFlaggedPBs(pb_list, (char)2);
		
		if(!top->ns->keepDeadEnd || !bot->ns->keepDeadEnd){
					Res->killDeadEndPores();
		}
        Res->rebuildThroatCounters();

		// Write network to file(s)
	    vtkFile = cFile + Res->ns->name + ".vtk";
		writeVTK(vtkFile, Res, Res->pb_sizeList, true);
		writeNetworkSpecs(cFile.c_str(), Res);
		writeInterfacePores(cFile.c_str(), Res, combi);
		writeConnectivity(cFile.c_str(), Res);
		writeLocation(cFile.c_str(), Res);
		
    	std::cout << "Done!" << std::endl;
		
    }
    else{

        PoreNetwork *inner = new PoreNetwork(name);

        std::string path = name.substr(0, (name.length() - 15));

        std::string nSpecs  = path + "NetworkSpecs.in";
        std::string cFile   = path;
        std::string fcFile  = path;
        std::string lFile   = path;
        std::string vtkFile = path;

        //writeVTK(vtkFile.c_str(), inner);
        if(Finalize(inner) != 0){
            std::cerr << "Something bad happend. \nI am sorry but, I QUIT!";
            return -1;
        }
    }
    
}


int Finalize(PoreNetwork * pn){

    std::string suffix;

    for(int dir = 0; dir <= 2; dir++){
        if(pn->ns->flowDirs[dir]){

            switch (dir) {
                case 0:
                    suffix = "_x";
                    break;

                case 1:
                    suffix = "_y";
                    break;

                case 2:
                    suffix = "_z";
                    break;
            }

            PoreNetwork *P_Bound = new PoreNetwork(*pn, pn->ns->name + suffix);

            P_Bound->generateBoundary(dir, P_Bound->ns->meanPBsize, P_Bound->ns->meanPBsize, pn->ns->pbDist, pn->ns->pbDist);

            size_t lengthTL = P_Bound->generateFullConnectivity();

            char * pb_list = searchForIsolatedPB(P_Bound,lengthTL);
            char * pb_list2 = searchForIsolatedPB_itertative(P_Bound, lengthTL);

            if(!pb_list || !pb_list2){
                std::cerr << "Have a NULL!" << std::endl;
                std::abort();
            }


            for(size_t i = 1; i <= P_Bound->nrOfActivePBs; i++){
                if(pb_list[i] != pb_list2[i]){
                    std::cout << pb_list[i] << '\t' << pb_list2[i] << std::endl;
                    std::cout << "List NOT the same!" << std::endl;
                    std::abort();
                }
            }


            if(!pb_list){
                std::cerr << "Network is Broken Aborting" << std::endl;
                return -1;
            }
            P_Bound->removeFlaggedPBs(pb_list, (char)2);

            // This works WAY faster when done after searching
            if(!P_Bound->ns->keepDeadEnd)
                P_Bound->killDeadEndPores();
            P_Bound->rebuildThroatCounters();

            vtkFile = cFile + P_Bound->ns->name + ".vtk";
            writeVTK(vtkFile, P_Bound, P_Bound->pb_sizeList);
            writeConnectivity(cFile.c_str(), P_Bound);

            writeLocation(lFile.c_str(), P_Bound);
            writeNetworkSpecs(cFile.c_str(), P_Bound);

            delete P_Bound;
            std::cout << std::endl;
        }
    }

    return 0;
}

