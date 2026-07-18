#include "alphabet.h"
#include <stddef.h>
#include <stdlib.h>

// Global alphabet configuration
AlphabetConfig alphabetConfig = { NULL, -1 };

// Initialize the alphabet configuration
char InitializeAlphabetConfig()
{
    // Free existing memory if present
    if (alphabetConfig.alphabets != NULL)
    {
        free(alphabetConfig.alphabets);
        alphabetConfig.alphabets = NULL;
    }
    
    // Reset count
    alphabetConfig.count = -1;
    
    // Allocate memory for alphabets
    alphabetConfig.alphabets = (Alphabet*)calloc((size_t)MAX_ALPHABET_COUNT, sizeof(Alphabet));
    if (alphabetConfig.alphabets == NULL)
    {
        return -1; // Memory allocation failed
    }
    
    return 0;
}

// Get alphabet by ID
Alphabet* GetAlphabetById(char id)
{
    char i;
    
    for (i = 0; i <= alphabetConfig.count; i++)
    {
        if (alphabetConfig.alphabets[i].id == id)
            return &alphabetConfig.alphabets[i];
    }
    
    return NULL;
}

// Convert a numeric index to the corresponding symbol in the alphabet
// Returns the character at the given index, or 0 if index is out of range
char IndexToSymbol(Alphabet* alphabet, char index)
{
    if (alphabet == NULL)
        return 0;
    
    if (index < 0 || index >= alphabet->size)
        return 0;
    
    return alphabet->symbolMap[index];
}

// Convert a symbol to its numeric index in the alphabet
// Returns the index (0-based), or -1 if the symbol is not found
char SymbolToIndex(Alphabet* alphabet, char symbol)
{
    char i;
    
    if (alphabet == NULL)
        return -1;
    
    for (i = 0; i < alphabet->size; i++)
    {
        if (alphabet->symbolMap[i] == symbol)
            return i;
    }
    
    return -1;  // Symbol not found in alphabet
}

// Initialize an alphabet with a given size and symbol map
char InitializeAlphabet(Alphabet* alphabet, char size, const char* symbols)
{
    char i;
    
    if (alphabet == NULL || symbols == NULL)
        return -1;
    
    if (size < 1 || size > MAX_ALPHABET_SIZE)
        return -1;
    
    alphabet->size = size;
    
    // Copy symbol map
    for (i = 0; i < size; i++)
    {
        if (symbols[i] == 0)
            return -1;  // Symbol string too short
        alphabet->symbolMap[i] = symbols[i];
    }
    
    return 0;
}
char Alpha2Num(Alphabet* alphabet, char symbol)
{
    if (alphabet == NULL)
        return -1;
    
    for (char i = 0; i < alphabet->size; i++)
    {
        if (alphabet->symbolMap[i] == symbol)
            return i;
    }
    
    return -1; // Symbol not found
}
char Num2Alpha(Alphabet* alphabet, char index)
{
    if (alphabet == NULL)
        return 0;
    
    if (index < 0 || index >= alphabet->size)
        return 0;
    
    return alphabet->symbolMap[index];
}
