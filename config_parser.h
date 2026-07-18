//Configuration Parser Header File
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

// Parser state enumeration
typedef enum
{
    PARSE_STATE_INITIAL = 0,      // Waiting for [CONFIG]
    PARSE_STATE_CONFIG = 1,       // Parsing [CONFIG] section
    PARSE_STATE_ROTORS = 2,       // Parsing [ROTORS] section
    PARSE_STATE_MACHINES = 3,     // Parsing [MACHINES] section
    PARSE_STATE_ALPHABETS = 4     // Parsing [ALPHABETS] section
} ParseState;

extern char parseState;
extern char substate;
extern char configVersion;
char InitilizeParser();
char ParseLine(char* line);
char ParseConfigLine(char* line);
char ParseRotorLine(char* line);
char ParseAlphabetLine(char* line);
char CheckStateTransition(char* line);
char getWhiteCharIndex(char* string);
#endif