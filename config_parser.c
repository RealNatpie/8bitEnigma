#include "config_parser.h"
#include "parser_errors.h"
#include "rotors.h"
#include "wiring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "alphabet.h"
#include "machine.h"

char parseState = 0;
char substate = 0;
char configVersion = 0;
static Rotors* currentRotor = NULL; // Current rotor being parsed

char InitilizeParser()
{
    parseState = PARSE_STATE_INITIAL;
    substate = 0;
    configVersion = 0;
    
    // Initialize all configuration containers
    if (InitializeAlphabetConfig() != 0)
    {
        return PARSER_ERR_INTERNAL;
    }
    
    if (InitializeRotorConfig() != 0)
    {
        return PARSER_ERR_INTERNAL;
    }
    
    if (InitializeMachineConfig() != 0)
    {
        return PARSER_ERR_INTERNAL;
    }
    
    return 0;
}
char alocateRotorList()
{
    if (RotorList != NULL)
    {
        FreeRotorList(RotorList);
        RotorList = NULL;
    }

    RotorList = NULL; // Will be set when first rotor is added
    return PARSER_OK; // Success
}

char CheckStateTransition(char* line)
{
    char transition = 0; // No transition by default'
    if (strncmp(line, "[ROTORS]", 8) == 0)
	{
		substate = 0; //Reset the temp object count for the next section
		parseState = PARSE_STATE_ROTORS;
        transition = 1; // Indicate a transition occurred
	}
	if (strncmp(line, "[MACHINES]", 10) == 0)
	{
		substate = 0; //Reset the temp object count for the next section
		parseState = PARSE_STATE_MACHINES;
        transition = 1; // Indicate a transition occurred
	}
    if (strncmp(line, "[ALPHABETS]", 11) == 0)
	{
		substate = 0; //Reset the temp object count for the next section
		parseState = PARSE_STATE_ALPHABETS;
        transition = 1; // Indicate a transition occurred
	}
    return transition;
}


char ParseLine(char* line)
{
    //Parse the line based on the current parseState and substate
    //This is a placeholder implementation, actual parsing logic will depend on the specific requirements
    switch (parseState)
    {
        case PARSE_STATE_INITIAL: // Initial state
            if (strncmp(line, "[CONFIG]", 8) == 0)
            {
                substate = 0; // Initialize substate for config parsing
                parseState = PARSE_STATE_CONFIG; // Move to config parsing state
                return PARSER_OK;
            }
            return PARSER_ERR_EXPECTED_CONFIG; // Expected [CONFIG] section
        case PARSE_STATE_CONFIG: // Config parsing state
            return ParseConfigLine(line); // Call the config line parser
        case PARSE_STATE_ROTORS: // Rotor parsing state
            // Implement rotor parsing logic here
            return ParseRotorLine(line); // Call the rotor line parser
        case PARSE_STATE_MACHINES: // Machine parsing state
            return PARSER_OK; // Placeholder for now    
        case PARSE_STATE_ALPHABETS: // Alphabet parsing state
            // TODO: Implement alphabet parsing logic here
            return ParseAlphabetLine(line); // Call the alphabet line parser
       

        default:
            return -1; // Unknown state
    }
}
char ParseMachineLine(char* line)
{
    char* key;
    char* value;
    // Implement machine parsing logic here
    // This is a placeholder implementation, actual parsing logic will depend on the specific requirements
    if(CheckStateTransition(line) != 0)
    {
        return PARSER_OK; // Transition to a different state
    }
    switch (substate)
    {
        case 0: // Waiting for new machine definitions
            if (strncmp(line, "NewMachine", 10) == 0)
            {
                machineConfig.count++; // Move to the next machine index
                if (machineConfig.count >= MAX_AVAILABLE_MACHINES)
                {
                    return PARSER_ERR_INTERNAL; // Exceeded max machine count
                }   
                substate = 1; // Move to the next substate for machine definition
                return PARSER_OK;
            }
            break;
        case 1: // Defining a machine
            // Check for new machine or state transition
            if (strncmp(line, "NewMachine", 10) == 0)
            {
                machineConfig.count++; // Move to the next machine index
                if (machineConfig.count >= MAX_AVAILABLE_MACHINES)
                {
                    return PARSER_ERR_INTERNAL; // Exceeded max machine count
                }
                return PARSER_OK;
            }
            if(strncmp(line,"NewReflector",10) == 0)
            {
                // Handle new reflector definition here
                substate = 3; // Move to reflector definition substate
                return PARSER_OK;
            }
            if(CheckStateTransition(line) != 0)
            {
                return PARSER_OK; // Transition to a different state
            }
            // Look for [DESCRIPTION] marker and tansition to new substate
            if (strncmp(line, "[DESCRIPTION]", 13) == 0)
            {
                substate = 2; // Move to the description substate
                return PARSER_OK;
            }
            // look for key=value pairs for the machine definition
            key = strtok(line, "=");
            if(key == NULL) // Was not a value pair
            {
                return PARSER_OK; // Skip this line, it was not a key=value pair
            }
            value = strtok(NULL, "=");
            if(value == NULL)
            {
                return PARSER_OK; // Skip this line, missing value
            }
            // Id
            if(strcmp(key, "Id") == 0)
            {
                machineConfig.machines[machineConfig.count].id = atoi(value);
                return PARSER_OK;
            }
            // Name
            else if(strcmp(key, "Name") == 0)
            {
				strncpy(machineConfig.machines[machineConfig.count].name, value, 31);
				machineConfig.machines[machineConfig.count].name[31] = '\0';
                return PARSER_OK;
            }
            //AlphabetId
            else if(strcmp(key, "AlphabetId") == 0)
            {
                char alphabetId = atoi(value);
                Alphabet* alpha = GetAlphabetById(alphabetId);
                if (alpha == NULL)
                {
                    return PARSER_ERR_INVALID_REFERENCE; // Referenced alphabet not found
                }
                machineConfig.machines[machineConfig.count].alphabet = alpha;
                return PARSER_OK;
            }
            // RotorCount
            else if(strcmp(key, "RotorCount") == 0)
            {
                machineConfig.machines[machineConfig.count].rotorCount = atoi(value);
                return PARSER_OK;
            }
        //availableRotors
            else if(strcmp(key, "AvailableRotors") == 0)
            {
                // Parse available rotors as a comma-separated list
                char* token = strtok(value, ",");
                char rotorIndex = 0;
                while (token != NULL && rotorIndex < MAX_AVAILABLE_ROTORS)
                {
                    machineConfig.machines[machineConfig.count].availableRotors[rotorIndex++] = atoi(token);
                    token = strtok(NULL, ",");
                }
                machineConfig.machines[machineConfig.count].availableRotorCount = rotorIndex;
                return PARSER_OK;
            }
            //EntryWheel
            else if(strcmp(key, "EntryWheel") == 0)
            {
                char cidx = getWhiteCharIndex(value);
                if (cidx == -1)
                {
                    return PARSER_ERR_INVALID_FORMAT; // No non-whitespace character found
                }
                if(isalpha(value[cidx]))
                {
                    WireStr2char(machineConfig.machines[machineConfig.count].entryWheel, value, machineConfig.machines[machineConfig.count].alphabet);
                }
                else if(isdigit(value[cidx]))
                {
                    csv2char(machineConfig.machines[machineConfig.count].entryWheel, value, machineConfig.machines[machineConfig.count].alphabet);
                }
                else
                {
                    return PARSER_ERR_INVALID_FORMAT; // Invalid format
                }
                return PARSER_OK;
            }
            else if(strcmp(key, "DefaultReflector") == 0)
            {
                machineConfig.machines[machineConfig.count].defaultReflector = atoi(value);
                return PARSER_OK;
            }
            else if(strcmp(key, "Plugboard") == 0)
            {
                machineConfig.machines[machineConfig.count].plugboard = (atoi(value) != 0);
                return PARSER_OK;
            }
            else if(strcmp(key, "ReflectorAdvances") == 0)
            {
                machineConfig.machines[machineConfig.count].reflectorAdvances = (atoi(value) != 0);
                return PARSER_OK;
            }
            else if(strcmp(key, "SteppingModel") == 0)
            {
                machineConfig.machines[machineConfig.count].steppingModel = atoi(value);
                return PARSER_OK;
            }
            
            break;

        case 2: // Reading machine description
            if (strncmp(line, "[END_DESCRIPTION]", 17) == 0)
            {
                substate = 1; // Return to machine definition substate
                return PARSER_OK;
            }
            break;
    }
            // Look for key=value pairs for the machine definition
    return PARSER_OK; // Placeholder for now
}
char ParseAlphabetLine(char* line)
{
    char* key;
    char* value;
    // Implement alphabet parsing logic here
    // This is a placeholder implementation, actual parsing logic will depend on the specific requirements
    if(CheckStateTransition(line) != 0)
    {
        return PARSER_OK; // Transition to a different state
    }
    switch (substate)
    {
        case 0: // Waiting for new alphabet definitions
            if (strncmp(line, "NewAlphabet", 11) == 0)
            {
                alphabetConfig.count++; // Move to the next alphabet index
                if (alphabetConfig.count >= MAX_ALPHABET_COUNT)
                {
                    return PARSER_ERR_INTERNAL; // Exceeded max alphabet count
                }   
                substate = 1; // Move to the next substate for alphabet definition
                return PARSER_OK;
            }
            break;
        case 1: // Defining an alphabet
            // Check for new alphabet or state transition
            if (strncmp(line, "NewAlphabet", 11) == 0)
            {
                alphabetConfig.count++; // Move to the next alphabet index
                if (alphabetConfig.count >= MAX_ALPHABET_COUNT)
                {
                    return PARSER_ERR_INTERNAL; // Exceeded max alphabet count
                }
                return PARSER_OK;
            }
            if(CheckStateTransition(line) != 0)
            {
                return PARSER_OK; // Transition to a different state
            }
            // Look for key=value pairs for the alphabet definition
            key = strtok(line, "=");
            if(key == NULL) // Was not a value pair
            {
                return PARSER_OK; // Skip this line, it was not a key=value pair
            }
            value = strtok(NULL, "=");
            if(value == NULL)
            {
                return PARSER_OK; // Skip this line, missing value
            }
            
            if(strcmp(key, "Id") == 0)
            {
                alphabetConfig.alphabets[alphabetConfig.count].id = atoi(value);
                return PARSER_OK;
            }
            else if(strcmp(key, "Name") == 0)
            {
				strncpy(alphabetConfig.alphabets[alphabetConfig.count].name, value, 31);
				alphabetConfig.alphabets[alphabetConfig.count].name[31] = '\0';
                return PARSER_OK;
            }
            else if(strcmp(key, "Size") == 0)
            {
                alphabetConfig.alphabets[alphabetConfig.count].size = atoi(value);
                return PARSER_OK;
            }
            else if(strcmp(key, "SymbolMap") == 0)
            {
                strncpy(alphabetConfig.alphabets[alphabetConfig.count].symbolMap, value, MAX_ALPHABET_SIZE);
                return PARSER_OK;
            }
            break;
        default:
            break;
    }
    return PARSER_OK; // Placeholder for now
}
char ParseConfigLine(char* line)
{
	char* key;
	char* value;
    //Parse the config line based on the current substate
    //This is a placeholder implementation, actual parsing logic will depend on the specific requirements
    //Check for transition to different State
	if (CheckStateTransition(line)!= 0)
    {
        goto exitstate; // If a transition occurred, exit the function to handle state transition
    }
    /*if (strncmp(line, "[ROTORS]", 8) == 0)
	{
		substate = 0; //Reset the temp object count for the next section
		parseState = 2;
        goto exitstate;
	}
	if (strncmp(line, "[MACHINES]", 10) == 0)
	{
		substate = 0; //Reset the temp object count for the next section
		parseState = 3;
        goto exitstate;
	}*/
    //split the line into key and value
    key = strtok(line, "=");
	if(key == NULL)//Was not a value Par
	{
		return PARSER_OK;//Skip this line, it was not a key=value pair
	}
    value = strtok(NULL, "=");
    if(strcmp(key, "FormatVersion") == 0)
    {
        configVersion = atoi(value);
        if (configVersion != 1)
        {
            return PARSER_ERR_VERSION; //Unsupported version
        }
        return PARSER_OK;
    }  
    return PARSER_OK;  //Unrecognized key we can ignore?
    exitstate:
    if(configVersion != 1)
    {
        return PARSER_ERR_VERSION; //Unsupported version
    }
    //initilize structs based on Defined Max Constnats
    // RotorList will be initialized as rotors are added via AddRotor()

    return PARSER_OK; // Success

}
char ParseRotorLine(char* line)
{
    char* key;
    char* value;
    int idValue;
    //Parse the rotor line based on the current substate
    switch (substate)
    {
        case 0: // Waiting for new rotor definitions
            if(CheckStateTransition(line) != 0)
            {
                return PARSER_OK; // Transition to a different state, no error
            }
            if(strncmp(line, "NewRotor", 8) == 0)
            {
                newRotor:
                // Initialize a new rotor
                currentRotor = AddRotor();
                if (currentRotor == NULL)
                {
                    return PARSER_ERR_INTERNAL; // Memory allocation failed
                }
                substate = 1; // Move to the next substate for rotor definition
                return PARSER_OK;
            }
        case 1: // Defining a rotor
        //Test for new rotor and state trasition
            if(strncmp(line, "NewRotor", 8) == 0)
            {
                goto newRotor;
            }
            if(CheckStateTransition(line) != 0)
            {
                return PARSER_OK; // Transition to a different state, no error
            }
            // Look for key=value pairs for the rotor definition
                        key = strtok(line, "=");
	        if(key == NULL)//Was not a value Par
	        {
		        return PARSER_OK;//Skip this line, it was not a key=value pair
	        }
	        value = strtok(NULL, "=");
            value[strcspn(value, "\r\n")] = '\0';
            //ID
            if(strncmp(key, "Id",2) == 0)
            {
                            idValue = atoi(value);
              if (idValue > 0 && idValue <= 255){
                currentRotor->id = (unsigned char)idValue;
                return PARSER_OK;
              }
            }
            if(strncmp(key, "Name",4) == 0)
            {
                                strncpy(currentRotor->name, value, 19);
                                currentRotor->name[19] = '\0'; // Ensure null-termination
                return PARSER_OK;
            }
            if(strncmp(key, "AlphabetId",10) == 0)
            {
                char alphabetId = atoi(value);
                Alphabet* alpha = GetAlphabetById(alphabetId);
                if (alpha == NULL)
                {
                    return PARSER_ERR_INVALID_REFERENCE; // Referenced alphabet not found
                }
                currentRotor->alphabet = alpha;
                return PARSER_OK;
            }
            if(strncmp(key, "Wiring",6) == 0)
            {
                if(isalpha(value[0]))
                {
                    WireStr2char(currentRotor->pin, value, currentRotor->alphabet);
                }
                return PARSER_OK;
            }
            if(strncmp(key, "Notch",5) == 0)
            {
                char notchCounter = 0;
                char cidx = getWhiteCharIndex(value);
                if (cidx == -1)
                {
                    return PARSER_ERR_INVALID_FORMAT; // No non-whitespace character found
                }
                if(isalpha(value[cidx]))
                {
                    notchCounter = WireStr2char(currentRotor->nextRotorIndex, value, currentRotor->alphabet);
                    currentRotor->notches = notchCounter+1; // Add 1 becasue 0 is a valid index and we need to account for the null terminator
                }
                else if(isdigit(value[cidx]))
                {
                    notchCounter = csv2char(currentRotor->nextRotorIndex, value, currentRotor->alphabet);
                    currentRotor->notches = notchCounter+1; // Add 1 becasue 0 is a valid index and we need to account for the null terminator
                }
                if(notchCounter < 0)
                {
                    return PARSER_ERR_INVALID_FORMAT; // Invalid notch format
                }
                else
                {
                return PARSER_OK;
                }
            }
    }

    return PARSER_OK; // Success
}
char getWhiteCharIndex(char* string)//Returns the index of the first non-whitespace character, or -1 if none found
{
    char i = 0;
    while (string[i] != '\0')
    {
        if (!isspace((unsigned char)string[i]))
        {
            return i;
        }
        i++;
    }
    return -1; // Return -1 if no non-whitespace character is found
}