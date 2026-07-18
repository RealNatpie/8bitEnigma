#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include "alphabet.h"

// MAX_ALPHABET_SIZE must be defined in platform-specific main file before including this header
// Sets the maximum alphabet size for struct arrays
#ifndef MAX_ALPHABET_SIZE
#define MAX_ALPHABET_SIZE 40  // Default fallback for IDE/standalone compilation
#endif

typedef struct
{
	unsigned char pin[MAX_ALPHABET_SIZE];	//Map of pins for the plugboard.   Index is the input letter, value is the output letter.
	Alphabet* alphabet;	//Pointer to the alphabet for this plugboard.
    char initalized; // Flag to indicate if the plugboard has been initialized
    unsigned char pairCount; // Count of the number of pairs added to the plugboard
}PlugBoards;

void InitilizePlugboard(PlugBoards* Plugboard, Alphabet* alphabet);
char AddPBPair(PlugBoards* Plugboard, char input1, char input2);
char PBRremovePair(PlugBoards* Plugboard, char pair);
#endif