// EnigamTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma output CLIB_MALLOC_HEAP_SIZE = 2048

#define MAX_ALPHABET_SIZE 26  // CP/M: Limited memory, use minimum

#include<stdio.h>
#include "../rotors.h"
#include <cpar.h>
#include "CPMText.h"
#include "plugboard.h"
#include "../parser_errors.h"

int main()
{
	char result =	EnigmaInitilize();
	clearScreen();
	printFileContents("encpsp.scn");
	
	printCenteredText("Enigma CP/M", 80);
	//printf("Enigma CP/M\n");
	printCenteredText("By Nathanael Nunes 2026", 80);
	printf("Loading...\n");
	if(EnigmaInitilize() != PARSER_OK)
	{
		printf("Error in confiuration file.\n");
		return 1;
	}
	//printf("By Nathanael Nunes 2026\n");
	printf("Config file version: %d\n", configVersion);
	printf("Rotor struct size: %d bytes\n", (int)sizeof(Rotors));
	printf("Expected pin array size: %d\n", MAX_ALPHABET_SIZE);
	
	return 0;
}

