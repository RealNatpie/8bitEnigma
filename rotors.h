#ifndef ROTORS_H
#define ROTORS_H

#include "alphabet.h"

// MAX_ALPHABET_SIZE must be defined in platform-specific main file before including this header
// Sets the maximum alphabet size for struct arrays
#ifndef MAX_ALPHABET_SIZE
#define MAX_ALPHABET_SIZE 40  // Default fallback for IDE/standalone compilation
#endif

typedef struct Rotors
{
	char id;	//ID of the rotor.
	char name[20];	//Name of the rotor.
	unsigned char pin[MAX_ALPHABET_SIZE];	//Map of pins for the router.   Index is the input letter, value is the output letter.
	Alphabet* alphabet;	//Pointer to the alphabet for this rotor.
	char CurrentPosition;	//Current position of the rotor.
	char nextRotorIndex[4];	//Index of the next rotor in the sequence.
	char notches; // Number in index of the notch position(s) for the rotor.
	struct Rotors* next;	//Pointer to next rotor in list.
} Rotors;
typedef struct 
{
	unsigned char pin[MAX_ALPHABET_SIZE];	//Map of pins for the reflector.   Index is the input letter, value is the output letter.
	Alphabet* alphabet;	//Pointer to the alphabet for this reflector.
}Reflector;
typedef struct inputRotor
{
	unsigned char pin[MAX_ALPHABET_SIZE];	//Map of pins for the input rotor.   Index is the input letter, value is the output letter.
	Alphabet* alphabet;	//Pointer to the alphabet for this input rotor.
} InputRotor;


typedef struct 
{
	Rotors rotors[5];	//Array of rotors.
}RotorSets;

// Container for rotor list
typedef struct
{
    Rotors* rotors;     // Linked list head of rotors
    char count;         // Current count of rotors (-1 = uninitialized)
} RotorConfig;

extern Rotors* RotorList;
extern RotorSets RotorSet;
extern RotorConfig rotorConfig;

char InitializeRotorConfig();
Rotors* AddRotor();
void FreeRotorList(Rotors* head);
Rotors* GetRotorById(char id);

char EnigmaInitilize();
char LoadEnigmas();
char testIndex(Rotors* rotor, char index);
char rotorForward(Rotors* rotor, char input);
char rotorBackward(Rotors* rotor, char input);

#endif
