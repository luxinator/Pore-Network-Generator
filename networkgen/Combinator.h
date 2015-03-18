/*
 * Combinator.h
 *
 *  Created on: Mar 16, 2015
 *      Author: lucas
 */

#ifndef COMBINATOR_H_
#define COMBINATOR_H_

#include "PoreNetwork.h"
#include <vector>
#include <utility>
#include <cstddef>

using namespace std;

class Combinator{

private:
	PoreNetwork *Top;
	PoreNetwork *Bot;

	PoreNetwork *Result;


// Example usages of vector of pair :
//	this->Boundary_Layer.push_back(make_pair(2,3));
//	this->Boundary_Layer[2].first + this->boundary_Layer.second;

	 vector<pair<int,int>> Boundary_Layer;

	 float Survival;
	 float Separation;
	 float SearchDist;

public:
	/* Public Constructor */
	Combinator(PoreNetwork *top, PoreNetwork *bot);
	/* Start the Combination Process */
	void Combine(short side);
	void builtConnectionList();

	void setSurvival(float survival);
	void setSeparation(float separation);
	void setSearchDist(float searchDist);
	PoreNetwork * getResult();

};
#endif /* COMBINATOR_H_ */
