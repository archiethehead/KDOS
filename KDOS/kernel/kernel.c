void print_char(char c) {
    __asm {
        mov ah, 0x0E
        mov al, c
        int 0x10
    }
}

void print_string(char* string) {

    while (*string != '\0') {
        print_char(*string);
        string++;
    }

}

// This function will sit at the absolute top of the binary
void kernelMain(void) {
    
    char alphabet = 'a' - 1;
    
    do {

        alphabet++;
        print_char(alphabet);

    } while (alphabet != 'z');

    __asm {

        halt:
        jmp halt;

    }

}