#include "machine.h"
#include <stddef.h>
#include <stdlib.h>

// Global machine configuration
MachineConfig machineConfig = { NULL, -1 };

// Initialize the machine configuration structure
char InitializeMachineConfig()
{
    // Free existing memory if present
    if (machineConfig.machines != NULL)
    {
        free(machineConfig.machines);
        machineConfig.machines = NULL;
    }
    
    // Reset count
    machineConfig.count = -1;
    
    // Allocate memory for machines
    machineConfig.machines = (EnigmaMachine*)calloc((size_t)MAX_AVAILABLE_MACHINES, sizeof(EnigmaMachine));
    if (machineConfig.machines == NULL)
    {
        return -1; // Memory allocation failed
    }
    
    return 0;
}

// Free machine configuration memory
char FreeMachineConfig()
{
    char i;
    
    if (machineConfig.machines != NULL)
    {
        // Free reflector lists for each machine
        for (i = 0; i <= machineConfig.count; i++)
        {
            FreeReflectorList(machineConfig.machines[i].reflectors);
            machineConfig.machines[i].reflectors = NULL;
        }
        
        free(machineConfig.machines);
        machineConfig.machines = NULL;
    }
    machineConfig.count = -1;
    return 0;
}

// Get machine by ID
EnigmaMachine* GetMachineById(char id)
{
    char i;
    
    for (i = 0; i <= machineConfig.count; i++)
    {
        if (machineConfig.machines[i].id == id)
            return &machineConfig.machines[i];
    }
    
    return NULL;
}

// Get machine by index
EnigmaMachine* GetMachineByIndex(char index)
{
    if (index < 0 || index > machineConfig.count)
        return NULL;
    
    return &machineConfig.machines[index];
}

// Get reflector by ID from a specific machine
MachineReflector* GetReflectorById(EnigmaMachine* machine, char reflectorId)
{
    MachineReflector* current;
    
    if (machine == NULL)
        return NULL;
    
    current = machine->reflectors;
    while (current != NULL)
    {
        if (current->id == reflectorId)
            return current;
        current = current->next;
    }
    
    return NULL;
}

// Add a new reflector to a machine's linked list
MachineReflector* AddReflectorToMachine(EnigmaMachine* machine)
{
    MachineReflector* newReflector;
    MachineReflector* current;
    
    if (machine == NULL)
        return NULL;
    
    // Allocate new reflector
    newReflector = (MachineReflector*)calloc(1, sizeof(MachineReflector));
    if (newReflector == NULL)
        return NULL; // Memory allocation failed
    
    // Initialize the node
    newReflector->next = NULL;
    
    // Add to end of list
    if (machine->reflectors == NULL)
    {
        machine->reflectors = newReflector;
    }
    else
    {
        current = machine->reflectors;
        while (current->next != NULL)
            current = current->next;
        current->next = newReflector;
    }
    
    machine->reflectorCount++;
    return newReflector;
}

// Free a reflector linked list
void FreeReflectorList(MachineReflector* head)
{
    MachineReflector* current = head;
    MachineReflector* next;
    
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}
