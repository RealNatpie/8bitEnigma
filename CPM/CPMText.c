#include<stdio.h>
#include<string.h>
void clearScreen()
{
    // Clear the screen using ANSI escape codes
    printf("\033[2J\033[H");
}
void printCenteredText(const char* text, int width)
{
    int textLength = strlen(text);
    int padding = (width - textLength) / 2;
    for (int i = 0; i < padding; i++)
    {
        printf(" ");
    }
    printf("%s\n", text);
}
char printFileContents(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return 1; // Error opening file
    }

    // Some CP/M libc builds may not start text streams at byte 0.
    // Force position to file start, then stream bytes directly.
    fseek(file, 0L, SEEK_SET);

    while (1)
    {
        int ch = fgetc(file);
        if (ch == EOF || ch == 0x1A)
        {
            break;
        }
        putchar(ch);
    }

    fclose(file);
    return 0; // Success
}