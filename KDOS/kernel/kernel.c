void print_char(char c) {

    __asm {

        mov ah, 0x0E
        mov al, c
        int 0x10

    }
    
}

char blocking_input() {

    char input = 0;

    __asm {
        mov ah, 0x00
        int 0x16
        mov input, al
    }

    return input;

}

void print_string(char* string) {

    while (*string != '\0') {

        print_char(*string);
        string++;

    }

}

// This function will sit at the absolute top of the binary
void kernelMain(void) {
    
    while (1) {

        char userInput = blocking_input();
        print_char(userInput);

    }

    __asm {

        halt:
        jmp halt;

    }

}