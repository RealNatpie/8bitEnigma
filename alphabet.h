#ifndef ALPHABET_H
#define ALPHABET_H

// MAX_ALPHABET_SIZE must be defined in platform-specific main file before including this header
// Sets the maximum alphabet size for struct arrays
#ifndef MAX_ALPHABET_SIZE
#define MAX_ALPHABET_SIZE 40  // Default fallback for IDE/standalone compilation
#endif

// Maximum number of alphabets that can be defined in configuration
#ifndef MAX_ALPHABET_COUNT
#define MAX_ALPHABET_COUNT 10  // Default fallback for IDE/standalone compilation
#endif

// Alphabet definition - shared by machines, rotors, reflectors, plugboards, etc.
typedef struct
{
    char id;                                // ID of the alphabet
    char name[32];                          // Name of the alphabet (e.g., "Standard English")
    char size;                              // Size of the alphabet (e.g., 26 for English)
    char symbolMap[MAX_ALPHABET_SIZE];      // Character sequence (e.g., "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
} Alphabet;

// Container for alphabet list
typedef struct
{
    Alphabet* alphabets;    // Dynamically allocated array of alphabets
    char count;             // Current count of alphabets (-1 = uninitialized)
} AlphabetConfig;

extern AlphabetConfig alphabetConfig;

// Initialize the alphabet configuration
char InitializeAlphabetConfig();

// Convert a numeric index to the corresponding symbol in the alphabet
// Returns the character at the given index, or 0 if index is out of range
char IndexToSymbol(Alphabet* alphabet, char index);

// Convert a symbol to its numeric index in the alphabet
// Returns the index (0-based), or -1 if the symbol is not found
char SymbolToIndex(Alphabet* alphabet, char symbol);

// Initialize an alphabet with a given size and symbol map
char InitializeAlphabet(Alphabet* alphabet, char size, const char* symbols);

// Get alphabet by ID
Alphabet* GetAlphabetById(char id);
char Alpha2Num(Alphabet* alphabet, char symbol);
char Num2Alpha(Alphabet* alphabet, char index);
#endif
