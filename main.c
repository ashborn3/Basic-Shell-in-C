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


    // Check for input/output redirection
    char *input_file = NULL;
    char *output_file = NULL;
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;

    for (int i = 0; i < argument_count; i++) {
        if (strcmp(arguments[i], "<") == 0) {
            input_file = arguments[i + 1];
            arguments[i] = NULL;
            break;
        } else if (strcmp(arguments[i], ">") == 0) {
            output_file = arguments[i + 1];
            arguments[i] = NULL;
            break;
        }
    }

    if (input_file != NULL) {
        input_fd = open(input_file, O_RDONLY);
        if (input_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
    }

    if (output_file != NULL) {
        output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        execvp(arguments[0], arguments);
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        if (input_fd != STDIN_FILENO) {
            close(input_fd);
        }

        if (output_fd != STDOUT_FILENO) {
            close(output_fd);
        }

        int status;
        waitpid(pid, &status, 0);
    }
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