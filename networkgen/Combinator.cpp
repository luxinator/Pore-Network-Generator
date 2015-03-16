#include "Combinator.h"
#include <iostream>
#include <random>

#include "PoreNetwork.h"
#include "ArrayFunctions.h"

/*
 * Combines two inner networks, try to keep it general! could later become important
 * to be able to chooses which sides to stick together
 */
Combinator::Combinator(PoreNetwork *top, PoreNetwork *bot){
    
	// The generated network, might under certain conditions generate overlapping pores!
	// NOT my problem!

	this->Top = top;
	this->Bot = bot;


	PoreNetwork *result = new PoreNetwork();
	float * t = new float[3 * (top->nrOfActivePBs + bot->nrOfActivePBs) + 3];

	for(size_t i = 0; i < (top->nrOfActivePBs + bot->nrOfActivePBs) * 3 + 3; i++)
		t[i] = 0.0f;

	result->locationList  = new float*[3];
	result->locationList[0] = t;
	result->locationList[1] = t + (top->nrOfActivePBs + bot->nrOfActivePBs);
	result->locationList[2] = t + 2* (top->nrOfActivePBs + bot->nrOfActivePBs);
	t = nullptr;

	int *temp = new int[2 * (top->nrOfActivePBs + bot->nrOfActivePBs) + 2];
	for(size_t i = 0; i < (top->nrOfActivePBs + bot->nrOfActivePBs) * 2 + 2; i++)
			temp[i] = 0;

	result->throatCounter = new int*[2];
	result->throatCounter[0] = temp;
	result->throatCounter[1] = temp + (top->nrOfActivePBs + bot->nrOfActivePBs) + 1;
	temp = nullptr;

	// This is waaay to small,
	/*
	int *temp = new int[2 * (top->nrOfConnections + bot->nrOfConnections) + 2];
	result->throatList = new int*[2];
	result->throatList[0] = temp;
	result->throatList[1] = temp + (top->nrOfConnections + bot->nrOfConnections) + 1;
	*/

	this->Result = result;

	std::cout << "Combinator Succesfully Created" << std::endl;

}














