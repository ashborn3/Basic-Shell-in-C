#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void execute_command(char *command) {
    char *arguments[MAX_ARGUMENTS];
    int argument_count = 0;

    // Tokenize the command into arguments
    char *token = strtok(command, " ");
    while (token != NULL && argument_count < MAX_ARGUMENTS) {
        arguments[argument_count++] = token;
        token = strtok(NULL, " ");
    }
    arguments[argument_count] = NULL;

    //Add Forking
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("Nitin's Shell ~> ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);

        // Remove trailing newline character
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strncmp(command, "cd ", 3) == 0) {
            char *path = command + 3;
            execute_cd(path);
        } else {
            execute_command(command);
        }
    }

    return 0;
}