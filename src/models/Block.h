/*
 * Block.h
 *
 *  Created on: Jun 17, 2011
 *      Author: static
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <string>

class Block {
public:
	//global defines

	Block();
	Block(int pUpper, int pLower);
	virtual ~Block();

	//getters and setters
	int getUpperPip();
	void setUpperPip (int pUpper);

	int getLowerPip();
	void setLowerPip(int pLower);

	string toString();


private:
	int upperPip;
	int lowerPip;
};

#endif /* BLOCK_H_ */
