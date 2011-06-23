#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H


//some function definitions
void dbManagerInit();
void dbManagerDestroy();


void addFact();
void removeFact();

void getFact();
void getFactsByArg();


#define DATABASEMANAGER_H
