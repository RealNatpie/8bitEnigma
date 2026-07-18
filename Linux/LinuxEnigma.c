// EnigamTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define MAX_ALPHABET_SIZE 40  // Linux: Plenty of memory, support extended alphabets

#include<stdio.h>
#include "../rotors.h"
int main()
{
	PlugBoards plugboard;
	InitilizePlugboard(&plugboard);
	EnigmaInitilize();
	printf("Enigma Linux\n");
	return 0;
}