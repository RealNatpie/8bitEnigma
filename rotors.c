#define _CRT_SECURE_NO_WARNINGS
#include "rotors.h"
#include "rotor_constants.h"
#include "parser_errors.h"
#include "wiring.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config_parser.h"

Rotors* RotorList = NULL;
RotorSets RotorSet;
RotorConfig rotorConfig = { NULL, -1 };

char InitilizeRotorSet();

// Initialize the rotor configuration
char InitializeRotorConfig()
{
    // Free existing list if present
    if (rotorConfig.rotors != NULL)
    {
        FreeRotorList(rotorConfig.rotors);
        rotorConfig.rotors = NULL;
    }
    
    // Free legacy RotorList
    if (RotorList != NULL)
    {
        FreeRotorList(RotorList);
        RotorList = NULL;
    }
    
    // Reset count
    rotorConfig.count = -1;
    
    return 0;
}

// Add a new rotor to the linked list
Rotors* AddRotor()
{
    Rotors* newRotor;
    Rotors* current;
    
    // Allocate new rotor
    newRotor = (Rotors*)calloc(1, sizeof(Rotors));
    if (newRotor == NULL)
        return NULL; // Memory allocation failed
    
    // Initialize the node
    newRotor->next = NULL;
    
    // Add to end of list
    if (rotorConfig.rotors == NULL)
    {
        rotorConfig.rotors = newRotor;
    }
    else
    {
        current = rotorConfig.rotors;
        while (current->next != NULL)
            current = current->next;
        current->next = newRotor;
    }
    
    // Also maintain legacy RotorList pointer at the head
    if (RotorList == NULL)
    {
        RotorList = rotorConfig.rotors;
    }
    
    rotorConfig.count++;
    return newRotor;
}

// Free a rotor linked list
void FreeRotorList(Rotors* head)
{
    Rotors* current = head;
    Rotors* next;
    
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

// Get rotor by ID
Rotors* GetRotorById(char id)
{
    Rotors* current = rotorConfig.rotors;
    
    while (current != NULL)
    {
        if (current->id == id)
            return current;
        current = current->next;
    }
    
    return NULL;
}

char EnigmaInitilize()
{
	char result = LoadEnigmas();
	if (result != PARSER_OK)
	{
		return result; //Return the error code from LoadEnigmas
	}
	return InitilizeRotorSet();

}
char InitilizeRotorSet()
{/*
	strcpy(RotorSet.rotors[0].name, "Rotor I");
	memcpy(RotorSet.rotors[0].pin, ROTOR_I_PIN, AlphabetSize);
	RotorSet.rotors[0].CurrentPosition = 0;
	RotorSet.rotors[0].nextRotorIndex[0] = 9;
	strcpy(RotorSet.rotors[1].name, "Rotor II");	
	memcpy(RotorSet.rotors[1].pin, ROTOR_II_PIN, AlphabetSize);
	RotorSet.rotors[1].CurrentPosition = 0;
	RotorSet.rotors[1].nextRotorIndex[0] = 9;
	strcpy(RotorSet.rotors[2].name, "Rotor III");
	memcpy(RotorSet.rotors[2].pin ,ROTOR_III_PIN, AlphabetSize);
	RotorSet.rotors[2].CurrentPosition = 0;
	RotorSet.rotors[2].nextRotorIndex[0] = 9;
	strcpy(RotorSet.rotors[3].name, "Rotor IV");
	memcpy(RotorSet.rotors[3].pin ,ROTOR_IV_PIN, AlphabetSize);
	RotorSet.rotors[3].CurrentPosition = 0;
	RotorSet.rotors[3].nextRotorIndex[0] = 9;
	strcpy(RotorSet.rotors[4].name, "Rotor V");
	memcpy(RotorSet.rotors[4].pin ,ROTOR_V_PIN, AlphabetSize);
	RotorSet.rotors[4].CurrentPosition = 0;
	RotorSet.rotors[4].nextRotorIndex[0] = 9;
	*/
	return PARSER_OK;
}
//LoadEnigmas loads enigma.cfg wich cotains engima and rotor deffinitions
//Returns 0 on success, 1 on failure.
char LoadEnigmas()
{
	char buffer[90];
	char state = 0; //0 = waiting for [CONFIG], 1 = waiting for [ROTORS], 2 = waiting for [MACHINES]
	char TempObjectCount = 0;
	char Version = 0;
	char result = 0;
	ParserErrorCode ErrorCode = PARSER_OK;
	//Try and open the file
	FILE* file = fopen("enigma.cfg", "r");
	if (file == NULL)	{
		return PARSER_ERR_FILE_OPEN; //Failed to open file
	}	
	//Parse the config file
	//Read current line until 80 charters or newline
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		//Check if the line is a comment or empty
		if (buffer[0] == ';' || buffer[0] == '\n')
		{
			continue; //Skip comments and empty lines
		}
		//Process the line
		//For now, just print it to the console
		//printf("%s", buffer);
		result = ParseLine(buffer);
		if (result != 0)
		{
			ErrorCode = result; //Set the error code from the parser
			goto cleanup; //Exit the loop and cleanup
		}
		/*
		switch (state)
		{
		case 0:  //Waiting for [CONFIG]
			if (strncmp(buffer, "[CONFIG]", 8) == 0)
			{
				state = 1;
			}
			else
			{
				ErrorCode = PARSER_ERR_EXPECTED_CONFIG; //Expected [CONFIG] section
				goto cleanup;
			}
			break;
		case 1:  //In the config section waiting for version number and alphabet size
			//Check for transition to different State
			if (strncmp(buffer, "[ROTORS]", 8) == 0)
			{
				TempObjectCount = 0; //Reset the temp object count for the next section
				state = 2;
			}
			if (strncmp(buffer, "[MACHINES]", 10) == 0)
			{
				TempObjectCount = 0; //Reset the temp object count for the next section
				state = 3;
			}
			//split the line into key and value
			char* key = strtok(buffer, "=");
			if(key == NULL)//Was not a value Par
			{
				break;//Skip this line, it was not a key=value pair
			}
			char* value = strtok(NULL, "=");
			if(strcmp(key, "FormatVersion") == 0)
			{
				Version = atoi(value);
				if (Version != 1)
				{
					ErrorCode = PARSER_ERR_VERSION; //Unsupported version
					goto cleanup;
				}
			}
			// Place holder for when I might care about this
			else if(strcmp(key, "AlphabetSize") == 0)
			{
				if (atoi(value) != 26)
				{
					ErrorCode = PARSER_ERR_INVALID_FORMAT; //Unsupported alphabet size
					goto cleanup;
				}
			}
			else if(strcmp(key, "SymbolMap") == 0)
			{
				if (strcmp(value, "A0Z25") != 0)
				{
					ErrorCode = PARSER_ERR_INVALID_FORMAT; //Unsupported symbol map
					goto cleanup;
				}
			}
			

			break;

		case 2:  // Waiting for new rotor definitions
			
			break;
		default:
			break;
		}
		*/
	}

cleanup:
	fclose(file);
	return ErrorCode; //Return 0 on success, or an error code on failure
}

//Calculate the output of a rotar from the input
char rotorForward(Rotors* rotor, char input)
{
	char inofset = (input + rotor->CurrentPosition) % rotor->alphabet->size;
	return (rotor->pin[inofset] + rotor->CurrentPosition) % rotor->alphabet->size;
}
//Calculate the output of a rotar from the reflector input
char rotorBackward(Rotors* rotor, char input)
{
	char inofset = (input + rotor->CurrentPosition) % rotor->alphabet->size;
	for(int i = 0; i < rotor->alphabet->size; i++)
	{
		if(rotor->pin[i] == inofset)
		{
			return (i + rotor->CurrentPosition) % rotor->alphabet->size	;
		}
	}
	return -1; //Not found
}
//Step the rotor and return 1 if the next rotor should advance
char rotorStep(Rotors* rotor)
{
	char shouldAdvanceNext = testIndex(rotor, rotor->CurrentPosition); //Check BEFORE stepping
	rotor->CurrentPosition = (rotor->CurrentPosition + 1) % rotor->alphabet->size	;
	return shouldAdvanceNext; //Return 1 if we were at a notch position
}
//Check if the rotor is at a notch position
char testIndex(Rotors* rotor, char index)
{
	for(int i = 0; i < rotor->notches; i++)
	{
		if(rotor->nextRotorIndex[i] == index)
		{
			return 1; //Index found in notches
		}
	}
	return 0; //Index not found in notches
}