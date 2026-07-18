// EnigamTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define MAX_ALPHABET_SIZE 26  // DOS 286: Limited memory, use minimum

#include<stdio.h>
#include "../rotors.h"
int main()
{
	PlugBoards plugboard;
	InitilizePlugboard(&plugboard);
	EnigmaInitilize();
	printf("Enigma DOS\n");
	return 0;
}
