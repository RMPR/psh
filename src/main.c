#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define PROMPT_STRING_SIZE 100
#define ARGC 64

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

    free(line);
    free(args);
}

char* psh_read_line(void) {
    // Read the line to put it in a string
    char* line = NULL;
    size_t len = 0;
    ssize_t chars_read;

    chars_read = getline(&line, &len, stdin);
    line[chars_read-1] = ' ';
    printf("p");
    for(int i=0; i < chars_read; i++) printf("s");
    printf("h");
    return line;

}

char** psh_split_args(char* line){
    // Split line in args to feed the execution function
    char** args = malloc(ARGC * sizeof(char*));
    int i = 0;

    if(!args) {
        fprintf(stderr, "Allocation failed");
        exit(EXIT_FAILURE);
    }

    for(char* arg = strtok(line, " "); arg != NULL; arg = strtok(NULL, " "), i++) {
        args[i] = strdup(arg);

        if(i == ARGC-1) {
            args = realloc(args, 2 * ARGC * sizeof(char*)); 
        }
    }

    return args;
}

int psh_execute_line(char** args){
    // Execute the command
    pid_t pid;
    int status = 0;

    if ((pid = fork()) < 0){
        fprintf(stderr, "Forking process failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if(execvp(args[0], args) < 0) {
            fprintf(stderr, "Execution of the command failed");
            _exit(EXIT_FAILURE);
        }
    } else {
        waitpid(pid, &status, 0);
    }

    return EXIT_SUCCESS;
}
