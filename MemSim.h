#ifndef MEMSIM_H
#define MEMSIM_H

#include <iostream>
#include <cstring>

// Function declarations
char readFromMemory(int virtualAddress);
void readFromMainMemory(int physicalPage, char *data);

#endif // MEMSIM_H