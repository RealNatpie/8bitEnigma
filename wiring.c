#include "wiring.h"
#include <string.h>
#include <stdlib.h>

// Convert a wiring string (e.g., "EKMFLGDQVZNTOWYHXUSPAIBRCJ") to numeric wire mapping
// Uses the provided alphabet to map characters to indices
// Returns the number of records written, or -1 on error
char WireStr2char(unsigned char* target, char* string, Alphabet* alphabet)
{
	int i;
	char records = 0;
	
	if (alphabet == NULL)
	{
		return -1; // Invalid alphabet
	}
	
	char num = 0;
	for (i = 0; i < alphabet->size; i++)
	{
		num = SymbolToIndex(alphabet, string[i]);
		if (num < 0)
		{
			return -1; // Invalid character in wiring
		}
		target[i] = num;
		records++;
	}

	return records; // Return the number of records written
}

// Convert a comma-separated value string (e.g., "1,5,9,13") to numeric array
// Returns the number of records written, or -1 on error
char csv2char(unsigned char* target, char* string, Alphabet* alphabet)
{
	int i;
	char tmpstr[4];
	char pntr = 0;
	char pairCount = 0;
	int slen = strlen(string);
	
	if (alphabet == NULL)
	{
		return -1; // Invalid alphabet
	}
	
	for (i = 0; i < slen; i++)
	{
		if (string[i] >= '0' && string[i] <= '9')
		{
			tmpstr[pntr++] = string[i];
		}
		else if (string[i] == ',' || string[i] == '\0' || string[i] == '\n')
		{
			if (pntr > 0)
			{
				tmpstr[pntr] = '\0';
				if (pairCount >= alphabet->size)
				{
					return -1; // Target array is too small
				}
				target[pairCount++] = (unsigned char)atoi(tmpstr);
				pntr = 0;
			}
			if (string[i] == '\0' || string[i] == '\n')
			{
				break; // End of string
			}
		}
	}
	
	if (pairCount < alphabet->size)
	{
		return -1; // Array is undersized - not enough values in CSV
	}
	
	return pairCount; // Return the number of records written
}
