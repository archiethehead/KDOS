#ifndef SHELL_H
#define SHELL_H

#define COMMAND_COUNT (sizeof(shellCommands) / sizeof(command))

void executeCommand(char* userInput);

void help();
void version(); 
void exit();
void reset();
void sysinfo();
void dir();
void cd();
void time();

typedef struct {

    char* name;
    void (*functionPointer)(void);
    char* description;

} command;

extern const command shellCommands[];

#endif // ifdef SHELL_H
