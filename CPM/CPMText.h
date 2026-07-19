#ifndef CPMTEXT_H
#define CPMTEXT_H
void clearScreen();
void printCenteredText(const char* text, int width);
char printFileContents(const char* filename);
void cursorUp(char lines);
void cursorDown(char lines);
void cursorRight(char lines);
void cursorLeft(char lines);
void cursorHome();
void setCursorPosition(char row, char col);
void clearLine();
#endif // CPMTEXT_H