/**
 * @Author: W.k.Tse
 * @Date : 6-3-'11
 * @Description: Command-line client interface
 */

#ifndef INTERFACE_H
#define INTERFACE_H

//define the functions when using the commandline-interface

/**
 * Print some start up info
 */
void printInfo();

/**
 * Get an input char from the input mechanism
 */
char getInputChar();

/**
 * Get an input number from the input method
 *
 * @param pString the string to print for giving user information
 * @pre pString is not null
 * @post pString is printed, and a number is returned
 */
int32_t getInputNumber (char *pString);

/**
 * For printing instruction for the user to see
 *
 * @param pIndex Selects which information to print
 * @pre pIndex is a positive number
 * @post The information that pIndex wants, are outputted
 */
void printInstruction (int32_t pIndex);


void printReadyForStart();

void printGameStarted();

void printGameEnded();

void printSetStarted();

void printSetEnded();

/**
 * Print a lineto the output
 * @param pString the string to print
 * @pre pString is not null
 * @post pString is printed on the output
 */
void printLine(char* pString);

#endif
