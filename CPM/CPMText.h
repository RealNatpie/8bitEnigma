#ifndef CPMTEXT_H
#define CPMTEXT_H
void clearScreen();
void printCenteredText(const char* text, int width);
char printFileContents(const char* filename);
#endif // CPMTEXT_H