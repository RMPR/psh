#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define PROMPT_STRING_SIZE 100

char* psh_read_config();
void psh_loop(char* prompt_string);
char* psh_read_line(void);
char** psh_split_args(char* line);
int psh_execute_line(char** args);

int main(int argc, char **argv) {
    // run the command loop reading the config file if any
    char prompt_string[PROMPT_STRING_SIZE] = "> ";
    psh_loop(psh_read_config(prompt_string));

    // exit succesfully the shell
    return EXIT_SUCCESS;
}

char* psh_read_config(char* prompt_string) {
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
    // Main loop of the Shell
    char* line = NULL;
    char** args = NULL;
    int status = 0;
    do {
        printf("%s", prompt_string);
        line = psh_read_line();        
        args = psh_split_args(line);
        status = psh_execute_line(args);
    } while (true);
}

char* psh_read_line(void) {
    // Read the line to put it in a string
    int buf_size = PROMPT_STRING_SIZE;
    char* line;
    int position = 0;
    char c;

    line = malloc((buf_size + 1) * sizeof(char));

    if (!line) {
        fprintf(stderr, "Memory allocation for the line failed");
    }
    line[buf_size] = '\0';

    while(true) {
        c = getchar();

        if (c == EOF || c == '\n'){
            return line;            
        } else {
            line[position] = c;
        }

        if(position == buf_size){
            buf_size = 2 * PROMPT_STRING_SIZE;
            line = realloc(line, buf_size * sizeof(char));

            if (!line) {
                fprintf(stderr, "Memory reallocation failed is the line too long?"); 
            }
        }
    }
}

char** psh_split_args(char* line){
    return NULL;
}

int psh_execute_line(char** args){
    return EXIT_SUCCESS;
}
