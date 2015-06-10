/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "Combinator.h"
#include <iostream>
#include <random>
#include <math.h>

#include "PoreNetwork.h"
#include "ArrayFunctions.h"


void Combinator::setSurvival(float survival)	{ this->Survival = survival; 	 }

void Combinator::setSeparation(float separation){ this->Separation = separation; }

void Combinator::setSearchDist(float searchDist){ this->SearchDist = searchDist; }

void Combinator::sortConnectionsList(bool toSort) { this->sortConnections = toSort; }

PoreNetwork * Combinator::getResult(){ return Result; };

/*
 * Combines two inner networks, try to keep it general! could later become important
 * to be able to chooses which sides to stick together
 */
Combinator::Combinator(PoreNetwork *top, PoreNetwork *bot, std::string name) {
    
	// The generated network, might under certain conditions generate overlapping pores!
	// NOT my problem!

	if(!top || !bot){
		std::cerr << "No PoreNetwork Objects Supplied to Combinator!" << std::endl;
		return;
	}

	this->Top = top;
	this->Bot = bot;

	this->SearchDist = -1.0f;
	this->Separation = -1.0f;
	this->Survival 	 = -1.0f;

	PoreNetwork *result = new PoreNetwork();
	result->ns = new NetworkSpecs;
	result->ns->name = name;
	result->ns->pbDist = Top->ns->pbDist;
	result->ns->Ni = 0;
	result->ns->Nj = 0;
	result->ns->Nk = 0;
	result->nrOfActivePBs = top->nrOfActivePBs + bot->nrOfActivePBs;

	float * t = new float[3 * result->nrOfActivePBs + 3];

	for(std::size_t i = 0; i < result->nrOfActivePBs * 3 + 3; i++)
		t[i] = 0.0f;

	result->locationList    = new float*[3];
	result->locationList[0] = t;
	result->locationList[1] = t + result->nrOfActivePBs + 1;
	result->locationList[2] = t + 2 * result->nrOfActivePBs + 2;
	
	result->pb_sizeList = new float[result->nrOfActivePBs + 1];
	// Copy over the Pb_sizes
	for(size_t i = 1 ; i <= bot->nrOfActivePBs; i++)
		result->pb_sizeList[i] = bot->pb_sizeList[i];
	
	for(size_t i = bot->nrOfActivePBs+1; i <= result->nrOfActivePBs; i++)
		result->pb_sizeList[i] = top->pb_sizeList[i - bot->nrOfActivePBs];
	
	result->ns->meanPBsize = bot->ns->meanPBsize;
	
	int *temp = new int[2 * result->nrOfActivePBs + 2];
	for(std::size_t i = 0; i < result->nrOfActivePBs * 2 + 2; i++)
			temp[i] = 0;

	result->throatCounter = new int*[2];
	result->throatCounter[0] = temp;
	result->throatCounter[1] = temp + result->nrOfActivePBs + 1;
	
	this->Result = result;
	std::cout << "Combinator Succesfully Created" << std::endl;

}


void Combinator::Combine(short side){


	if(side > 2 || side < 0){
		std::cerr<< "Invalid Side given in Combinator::Combine!" << std::endl;
		return;
	}
	if( this->Separation < 0.0f){
		std::cerr << "Please set the Separation Distance!" << std::endl;
		return;
	}
	if( this->SearchDist < 0.0f) {
		std::cerr << "Please set the Search Distance!" << std::endl;
		return;
	}
	if( this->Survival < 0.0f) {
			std::cerr << "Please set the Survival Chance!" << std::endl;
			return;
		}
	std::cout << "Separation Distance: " << Separation << std::endl;
	std::cout << "Seaching Distance: " << SearchDist << std::endl;
	std::cout << "Survival: " << Survival << std::endl;

	// ------ Location ------//
	float translation[] = { 0.0f, 0.0f, 0.0f };
	translation[side] = Separation + Bot->locationList[side][Bot->nrOfActivePBs];

	// add the lower half to the Result network

	for(std::size_t i = 1; i <= Bot->nrOfActivePBs; i++ ){
		this->Result->locationList[0][i] = this->Bot->locationList[0][i];
		this->Result->locationList[1][i] = this->Bot->locationList[1][i];
		this->Result->locationList[2][i] = this->Bot->locationList[2][i];
	}
	// add the top part to the Results network

	for (size_t i = 1; i <= Top->nrOfActivePBs; i++){
		this->Result->locationList[0][i + Bot->nrOfActivePBs] = this->Top->locationList[0][i] + translation[0];
		this->Result->locationList[1][i + Bot->nrOfActivePBs] = this->Top->locationList[1][i] + translation[1];
		this->Result->locationList[2][i + Bot->nrOfActivePBs] = this->Top->locationList[2][i] + translation[2];
	}

// Naive but simple approach, loop all pb's if at interface do tests
	 //Set up Random Generator
	    std::random_device r_dev{};
	    std::default_random_engine e{r_dev()};
	    std::uniform_real_distribution<float> d{0.0f,1.0f};

	for(std::size_t i = 1; i <= Bot->nrOfActivePBs; i++){
		if (Bot->locationList[side][i] == Bot->locationList[side][this->Bot->nrOfActivePBs]){
			// To keep it generic, we search ALL the porebodies

			for(std::size_t j = 1; j <= Top->nrOfActivePBs; j++){

				if( Top->locationList[side][j] == 0.0f){
					std::size_t pn = Bot->nrOfActivePBs + j;

					double L = sqrt(pow((double) (Result->locationList[0][i] - Result->locationList[0][pn]), 2) +
									pow((double) (Result->locationList[1][i] - Result->locationList[1][pn]), 2) +
									pow((double) (Result->locationList[2][i] - Result->locationList[2][pn]), 2) );
					if(L == 0.0f)
						std::cout << "CRAP!" << std::endl;

					if( L <= this->SearchDist && d(e) < Survival)
						Boundary_Layer.push_back( { (int)i, (int)pn } );
				}// if top[side] 0.0
			}// for Bot
		}// if side
	} // for Top

	// We now have a List of Boundary layer connections
	std::cout << "Nr of Interface Conns: " << Boundary_Layer.size() << std::endl;

}


void Combinator::builtConnectionList(){

	//Build Connections
	std::size_t nrOfInterFaceConns = Boundary_Layer.size();
	Result->nrOfConnections = Top->nrOfConnections + Bot->nrOfConnections + nrOfInterFaceConns;


	int *temp = new int[2 * Result->nrOfConnections + 2];
	Result->throatList = new int*[2];
	Result->throatList[0] = temp;
	Result->throatList[1] = temp + Result->nrOfConnections + 1;

	std::size_t i;
	// Bottom Part just a copy of Bottom
	for(i = 0; i < Bot->nrOfConnections; i ++){
		Result->throatList[0][i] = Bot->throatList[0][i];
		Result->throatList[1][i] = Bot->throatList[1][i];
	}
	// Add the Boundary Layer, already renumberd!
	for (std::vector<std::pair<int,int> >::iterator it = Boundary_Layer.begin(); it != Boundary_Layer.end(); it++){
		Result->throatList[0][i] = it->first;
		Result->throatList[1][i] = it->second;
		i++;
	}
	// Add the Top and Renumber
	for(i = 0; i < Top->nrOfConnections; i++){
		Result->throatList[0][i + Bot->nrOfConnections + nrOfInterFaceConns] = Top->throatList[0][i] + Bot->nrOfActivePBs;
		Result->throatList[1][i + Bot->nrOfConnections + nrOfInterFaceConns] = Top->throatList[1][i] + Bot->nrOfActivePBs;
	}
	
	// Sort 
	if(sortConnections)
		bubbleSortList(Result->throatList, Result->nrOfConnections);


}










