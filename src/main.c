#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define PROMPT_STRING_SIZE 100
#define ARGC 64
#define NUM_BUILT_INS 3

char* psh_read_config();
void psh_loop(char* prompt_string);
char* psh_read_line(void);
char** psh_split_args(char* line);
int psh_execute_line(char** args);
int psh_cd(char* path);
int psh_exit();
int psh_help();


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
    char wd[PATH_MAX];

    do {
        printf("%s", prompt_string);
        line = psh_read_line();        
        args = psh_split_args(line);
        status = psh_execute_line(args);
        getcwd(wd, PATH_MAX);
        printf("%s", wd);
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

    return line;

}

char** psh_split_args(char* line) {
    // Split line in args to feed the execution function
    char** args = malloc(ARGC * sizeof(char*));
    int i = 0;

    if (!args) {
        fprintf(stderr, "Allocation failed");
        exit(EXIT_FAILURE);
    }

    for(char* arg = strtok(line, " "); arg != NULL; arg = strtok(NULL, " "), i++) {
        args[i] = strdup(arg);

        if (i == ARGC-1) {
            args = realloc(args, 2 * ARGC * sizeof(char*)); 
        }
    }

    return args;
}

int psh_execute_line(char** args) {
    // Execute the command
    pid_t pid;
    int status = 0;
    
    // Verify beforehand that it's not a built-in
    if (strcmp(args[0], "cd") == 0) {
        return psh_cd(args[1]);
    } else if (strcmp(args[0], "exit") == 0)
        exit(psh_exit());
    else if (strcmp(args[0], "help") == 0)
        return psh_help();

    if ((pid = fork()) < 0) {
        fprintf(stderr, "Forking process failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (execvp(args[0], args) < 0) {
            fprintf(stderr, "Execution of the command failed");
            _exit(EXIT_FAILURE);
        }
    } else {
        waitpid(pid, &status, 0);
    }

    return EXIT_SUCCESS;
}

int psh_cd(char* path) {
    int status = 0;
    if ((status = chdir(path)) != 0) {
        fprintf(stderr, "\n Error when changing directories \n"); 
    }
    return status;
}

int psh_exit(){
    return EXIT_SUCCESS;
}

int psh_help() {
    printf("RMPR psh version whatever");     
    return EXIT_SUCCESS;
}
