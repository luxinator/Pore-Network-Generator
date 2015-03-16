/*
 * Combinator.h
 *
 *  Created on: Mar 16, 2015
 *      Author: lucas
 */

#ifndef COMBINATOR_H_
#define COMBINATOR_H_

#include "PoreNetwork.h"

class Combinator{

private:
	PoreNetwork *Top;
	PoreNetwork *Bot;
	PoreNetwork *Result;

	 float Survival;
	 float Separation;
	 float SearchDist;

public:
	Combinator(PoreNetwork *top, PoreNetwork *bot);


};
#endif /* COMBINATOR_H_ */
