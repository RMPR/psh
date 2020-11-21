#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define PROMPT_STRING_SIZE 100

char* read_config();
void psh_loop(char* prompt_string);

int main(int argc, char **argv) {
    // run the command loop reading the config file if any
    char prompt_string[PROMPT_STRING_SIZE] = "> ";
    psh_loop(read_config(prompt_string));

    // exit succesfully the shell
    return EXIT_SUCCESS;
}

char* read_config(char* prompt_string) {
    // The config consists in the char displayed in the shell
    // TODO: Input control
    FILE* config_file = fopen("psh.cfg", "r");

    if (config_file != NULL) {
        fgets(prompt_string, PROMPT_STRING_SIZE-1, config_file);
        fclose(config_file);
    } else {
        printf("No config file found: Creating one... \n");
        config_file = fopen("psh.cfg", "w");
        fprintf(config_file, "%s", prompt_string);
        fclose(config_file);
    }

    return prompt_string;
}

void psh_loop(char* prompt_string) {
    do {
        printf("%s", prompt_string);
        getchar(); 
    } while (true);
}
