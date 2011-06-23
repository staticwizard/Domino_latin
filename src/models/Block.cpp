/*
 * Block.cpp
 *
 *  Created on: Jun 17, 2011
 *      Author: static
 */
#include <assert.h>
#include "../defines.h"
#include <sstream>

#include "Block.h"

Block::Block() {

}

Block::Block(int pUpper, int pLower) {
	setUpperPip (pUpper);
	setLowerPip(pLower);
}

Block::~Block() {

}

Block::getLowerPip() {
	return lowerPip;
}

Block::setLowerPip(int pLower) {
	assert (pLower >= 0);
	assert (pLower <= MAX_PIP);
	lowerPip = pLower;
}

Block::getUpperPip() {
	return upperPip;
}

Block::setUpperPip(int pUpper) {
	assert (pUpper >= 0);
	assert (pUpper <= MAX_PIP);

	upperPip = pUpper;
}

Block::toString() {
	stringstream buff;
	buff << "Upper pip: " << getUpperPip() << " Lower pip: " << getLowerPip() << "\n";
}
