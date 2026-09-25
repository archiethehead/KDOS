#ifndef SHELL_H
#define SHELL_H

#define COMMAND_COUNT (sizeof(shellCommands) / sizeof(command))

void executeCommand(const char* userInput);

void exit();
void sysinfo();
void dir();

typedef struct {

    char name[10];
    void (*functionPointer)(void);

} command;

extern const command shellCommands[];

#endif // ifdef SHELL_H
