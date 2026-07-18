#ifndef MACHINE_H
#define MACHINE_H

#include "alphabet.h"
#include <string.h>

typedef struct Rotors Rotors;

// MAX_ALPHABET_SIZE must be defined in platform-specific main file before including this header
// Sets the maximum alphabet size for struct arrays
#ifndef MAX_ALPHABET_SIZE
#define MAX_ALPHABET_SIZE 40  // Default fallback for IDE/standalone compilation
#endif

// Maximum number of rotor IDs that can be available for a machine
#ifndef MAX_AVAILABLE_ROTORS
#define MAX_AVAILABLE_ROTORS 10
#endif

// Maximum description length
#ifndef MAX_DESCRIPTION_LENGTH
#define MAX_DESCRIPTION_LENGTH 512
#endif
// Maximum number of machines that can be configured
#ifndef MAX_AVAILABLE_MACHINES
#define MAX_AVAILABLE_MACHINES 10
#endif

// Reflector definition within a machine (linked list node)
typedef struct MachineReflector
{
    char id;                                    // ID of the reflector
    char name[20];                              // Name of the reflector (e.g., "UKW B")
    unsigned char wiring[MAX_ALPHABET_SIZE];    // Reflector wiring mapping
    Alphabet* alphabet;                         // Pointer to the alphabet for this reflector
    char rotates;                               // Boolean: can this reflector rotate?
    char rewirable;                             // Boolean: is this reflector rewirable?
    char thin;                                  // Boolean: is this a thin reflector?
    struct MachineReflector* next;              // Pointer to next reflector in list
} MachineReflector;

// Enigma machine configuration
typedef struct
{
    char id;                                    // Machine ID
    char name[32];                              // Machine name (e.g., "Enigma M3")
    Alphabet* alphabet;                         // Pointer to the alphabet for this machine
    char rotorCount;                            // Number of active stepping rotors
    char availableRotors[MAX_AVAILABLE_ROTORS]; // Array of rotor IDs available for this machine
    char availableRotorCount;                   // Number of available rotors
    unsigned char entryWheel[MAX_ALPHABET_SIZE];// Entry wheel (ETW) wiring
    char defaultReflector;                      // ID of the default reflector
    MachineReflector* reflectors;               // Linked list of reflectors for this machine
    Rotors* instaledRotors;                     // Array of pointers to rotors installed during operation
    MachineReflector* instaledReflector;       // Pointer to the reflector installed during operation
    char reflectorCount;                        // Number of reflectors defined
    char plugboard;                             // Boolean: does this machine support plugboard?
    char reflectorAdvances;                     // Boolean: does reflector advance?
    char steppingModel;                         // Stepping model ID (parser/runtime-defined)
} EnigmaMachine;

// Machine configuration list/catalog
typedef struct
{
    EnigmaMachine* machines;    // Dynamically allocated array of machines
    char count;                 // Current count of machines (-1 = uninitialized)
} MachineConfig;

extern MachineConfig machineConfig;

// Function declarations for machine operations
char InitializeMachineConfig();
char LoadMachines();
char FreeMachineConfig();
EnigmaMachine* GetMachineById(char id);
EnigmaMachine* GetMachineByIndex(char index);
MachineReflector* GetReflectorById(EnigmaMachine* machine, char reflectorId);
MachineReflector* AddReflectorToMachine(EnigmaMachine* machine);
void FreeReflectorList(MachineReflector* head);

#endif
