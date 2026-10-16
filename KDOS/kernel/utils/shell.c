#include "console-io.h"
#include "shell.h"
#include "disk.h"
#include "memory.h"
#include "time.h"
#include "arg.h"

#define MAX_ARGS 16 

extern char version_type[];
extern uint8_t version_major;
extern uint8_t version_minor;
extern uint8_t version_patch;
extern char version_name[];

char* argv[MAX_ARGS];
uint8_t argc = 0;
bool verbose = false;

const command shellCommands[] = {

    {"help",    &help,      "[command name(s)] Outputs every command and it's arguments"},
    {"version", &version,   "Outputs the KDOS version number"},
    {"exit",    &exit,      "Shuts down KDOS and the host machine"},
    {"reset",   &reset,     "Shuts down KDOS and reboots the host machine"},
    {"sysinfo", &sysinfo,   "Outputs the hardware statistics of the host machine"},
    {"dir",     &dir,       "Outputs the contents of the current directory"},
    {"cd",      &cd,        "<directory name> Changes the current directory (^ = parent directory)"},
    {"cls",     &clrscr,    "Clears the screen of all text"},
    {"time",    &time,      "Outputs the current time"},
    {"type",    &type,      "<-f filename> Outputs the text contents of a file"}

};

void printFunc(command Command) {

    char* gap = "          ";
    uint16_t len = 8 - strlen(Command.name);
    gap[len] = '\0';

    printString(Command.name);
    printString(gap);
    printString(Command.description);
    newline();

    gap[len] = ' ';

}

void help() {

    printString("\nCOMMANDS\n\n<> - Mandatory Argument(s)\n[] - Optional Argument(s)\n"
                "Arguments specified with a '-' flag aren't order sensitive\n"
                "View verbose output with the '-v' flag (where applicable)\n\n");

    for (int i = 0; i < COMMAND_COUNT; i++) {

        if (argc == 1)
            printFunc(shellCommands[i]);

        else {

            for (int j = 1; j < argc; j++)
                if (strequal(argv[j], shellCommands[i].name))
                    printFunc(shellCommands[i]);

        }

    }

}

void version() {

    newline();
    printString(version_type);
    printString(" Version ");
    printUint(version_major);
    printChar('.');
    printUint(version_minor);
    printChar('.');
    printUint(version_patch);
    newline();

}

void exit() {

    __asm {

        mov ax, 0x5307
        mov bx, 0x0001
        mov cx, 0x0003
        int 0x15

    }

}

void reset() {

    clrscr();

    __asm {

        int 0x19

    }

}

void sysinfo() {

    newline();

    uint32_t memory = 1048576L; //1 MiB
    uint16_t oneKibBetween1and16Meg = 0;
    uint16_t sixtyFourKibBeyond16Meg = 0;

    __asm {

        mov ax, 0xE801
        int 0x15
        mov oneKibBetween1and16Meg, ax
        mov sixtyFourKibBeyond16Meg, bx

    }

    uint32_t ULoneKibBetween1and16Meg = (uint32_t)oneKibBetween1and16Meg * 1024UL;    // to bytes
    uint32_t ULsixtyFourKibBeyond16Meg = (uint32_t)sixtyFourKibBeyond16Meg * 65536UL; // to bytes

    memory += ULoneKibBetween1and16Meg;
    memory += ULsixtyFourKibBeyond16Meg;
    memory = memory >> 20; // x >> 20 == x / 1048576L

    char numberBuffer[32];
    printUint((uint64_t)memory);
    printString(numberBuffer);
    printString(" MiB of extended-memory\n");
     
    uint16_t conventionalMemory = 0;
    __asm {

        int 0x12
        mov conventionalMemory, ax
    
    }
    
    printUint(conventionalMemory);
    printString(" KiB of usable memory\n");
    printUint(hardDriveCount);
    printString(" Hard Drive(s) connected\n");
    printUint(floppyDriveCount);
    printString(" Floppy Drive(s) connected\n");

}

void dir() {

    char* line = "-------";

    newline();
    int len = strlen(currentDirectory.metadata.directoryName);
    int files = 0;
    int folders = 0;
    int programs = 0;

    line[len] = '\0';
    
    printString(line);

    newline();
    printString(currentDirectory.metadata.directoryName);
    newline();

    printString(line);

    line[len] = '-';

    newline();

    char fileCount = currentDirectory.metadata.fileCount;
    for (uint16_t i = 0; i < fileCount; i++) {

        char* gap = "          ";
        int len =  strlen(currentDirectory.entries[i].fileName);
        len = 10 - len;
        gap[len] = '\0';
        printString(currentDirectory.entries[i].fileName);

        fileType flag = FILE_TYPE(currentDirectory.entries[i]);

        switch (flag) {

        case (file):

            printString(gap);
            printString("<FILE>");
            files++;
            break;

        case (folder):

            printString(gap);
            printString("<FOLDER>");
            folders++;

            break;

        case (executable):

            programs++;
            break;

        }

        gap[len] = ' ';
        newline();

    }

    printString("\n\n");

    printUint(files);
    printString(" File(s)\n");

    printUint(folders);
    printString(" Folder(s)\n");
    
    printUint(programs);
    printString(" Executable(s)\n");

}

void cd() {

    newline();

    if (argc < 2) {

        printString("No path was specified.\n");
        return;

    }

    if (argv[1][0] == '^') {

        if (openParentDirectory())
            printString("This folder is the root.\n");

        return;

    }

    if (openFile(argv[1], folder))
        return;

    printString("The folder/path specified does not exist.\n");

}

void time() {

    newline();
    printTime(getTime());
    newline();

}

void type() {

    char* fileName = nullptr;
    int opt = 0;

    while((opt = getopt(argc - 1, argv + 1, "f:v")) != -1) {

        switch(opt) {

            case 'v':
                verbose = true;
                break;
            
            case 'f':
                fileName = optarg;
                break;
            
            case '?':
                printChar(optopt);
                break;

        }

    }

    if (fileName == nullptr) {

        printString("No file was specified.\n");
        return;

    }

    newline();

    if (verbose) {
        
        printString("Loading ");
        printString(fileName);
        printString(" . . .\n");

    }

    if (openFile(fileName, file)) {
        
        if (verbose) {
        
            printString("Loaded ");
            printString(fileName);
            printString("!\n");

        }


        uint8_t huge* mutableMemory = fileMemory;
        uint32_t count = 0;

        while (*mutableMemory != '\0') {

            if (*mutableMemory == '\n')
                printChar('\r');

            printChar(*mutableMemory);
            count++;

            if (count == 80 || *mutableMemory == '\n') {

                count = 0;
                
                char input = blockingInput();

                if (input == 'q')
                    return;

            }

            mutableMemory++;

        }

        return;

    }

    printString("The file specified does not exist.\n");

}

void executeCommand(char* userInput) {

    verbose = false;
    optreset = true;

    for (int i = 0; i < MAX_ARGS; i++)
        argv[i] = 0;
    argc = 0;

    char* currentArg = userInput;

    while (*currentArg != '\0') {

        while (*currentArg == ' ')
            currentArg++;

        if (*currentArg == '\0')
            break;
        
        if (argc >= MAX_ARGS)
            break;

        argv[argc++] = currentArg;
        
        while (*currentArg != ' ' && *currentArg != '\0')
            currentArg++;

        if (*currentArg == ' ') {
         
            *currentArg = '\0';
            currentArg++;

        }

    }

    for (int i = 0; i < COMMAND_COUNT; i++) {

        if (strequal(userInput, shellCommands[i].name)) {
         
            shellCommands[i].functionPointer();
            return;

        }

    }

    printString(userInput);
    printString(" is not a recognised command\n");

}
