/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef COMBINATOR_H_
#define COMBINATOR_H_

#include "PoreNetwork.h"
#include <vector>
#include <utility>


class Combinator{

private:
	PoreNetwork *Top;
	PoreNetwork *Bot;

	PoreNetwork *Result;


// Example usages of vector of pair :
//	this->Boundary_Layer.push_back(make_pair(2,3));
//	this->Boundary_Layer[2].first + this->boundary_Layer.second;

	std::vector<std::pair<int,int>> Boundary_Layer;

	 float Survival;
	 float Separation;
	 float SearchDist;

public:
	/* Public Constructor */
	Combinator(PoreNetwork *top, PoreNetwork *bot, std::string name);
	
	/* Start the Combination Process */
	void Combine(short side);
	void builtConnectionList();

	void setSurvival(float survival);
	void setSeparation(float separation);
	void setSearchDist(float searchDist);
	PoreNetwork * getResult();
	
	const std::vector<std::pair<int,int>>& getInterface() { return Boundary_Layer; };
	
	float getSurvival()  { return Survival; };
	float getSeparation(){ return Separation; };
	float getSearchDist(){ return SearchDist; };

};
#endif /* COMBINATOR_H_ */
