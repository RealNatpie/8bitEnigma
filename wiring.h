#ifndef WIRING_H
#define WIRING_H

#include "alphabet.h"

// Convert a wiring string (e.g., "EKMFLGDQVZNTOWYHXUSPAIBRCJ") to numeric wire mapping
// Uses the provided alphabet to map characters to indices
// Returns the number of records written, or -1 on error
char WireStr2char(unsigned char* target, char* string, Alphabet* alphabet);

// Convert a comma-separated value string (e.g., "1,5,9,13") to numeric array
// Returns the number of records written, or -1 on error
char csv2char(unsigned char* target, char* string, Alphabet* alphabet);

#endif
