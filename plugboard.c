#include "plugboard.h"
#include "config_parser.h"
#include <stddef.h>

void InitilizePlugboard(PlugBoards* Plugboard, Alphabet* alphabet)
{
	int i;
	if (alphabet == NULL)
	{
		return; // Cannot initialize without alphabet
	}
	
	for (i = 0; i < alphabet->size; i++)
	{
		Plugboard->pin[i] = i;// set the defaul upluged state where the busbar is connected to the same letter
	}
	Plugboard->alphabet = alphabet; // Store the alphabet pointer
	Plugboard->initalized = 1; // Mark the plugboard as initialized
    Plugboard->pairCount = 0; // Initialize the pair count to 0
}
char AddPBPair(PlugBoards* Plugboard, char input1, char input2)
{
    if(Plugboard->initalized != 1)
    {
        // Cannot auto-initialize without alphabet - caller must initialize first
        return -2; // Plugboard not initialized
    }
    
	// Verify that these pins aren't already connected to something else
	if (Plugboard->pin[input1] != input1)
	{
		return -1; // input1 already has a plug connected
	}
	if (Plugboard->pin[input2] != input2)
	{
		return -1; // input2 already has a plug connected
	}
	
	Plugboard->pin[input1] = input2;
	Plugboard->pin[input2] = input1;
    Plugboard->pairCount++; // Increment the pair count
	return 0; // Success
}
char PBRremovePair(PlugBoards* Plugboard, char pair)//Removes a par useing indetifire forom ether side
{
    //Veerife that pair is valid and connected
    if(Plugboard->pin[pair] == pair)
    {
        return -1; // Pair is not connected
    }
    //Find the other end of the pair
    char otherEnd = Plugboard->pin[pair];
    //reset both ends to default shorted state
    Plugboard->pin[pair] = pair;
    Plugboard->pin[otherEnd] = otherEnd;
    //decrement the pair count
    if(Plugboard->pairCount > 0)
    {
        Plugboard->pairCount--;
    }
}