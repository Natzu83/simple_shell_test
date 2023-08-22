#include "simple_shell.h"

/**
 * execute - Function to execute a command using fork and execve
 *
 * @command: The command to execute
 *
 * @env: The environment variables
 */

void execute(char *command, char **env) {
    pid_t child_pid;
    char *token = NULL;
    char **tokens = NULL;
    int token_count = 0;

    /* Tokenize the command string */
    token = strtok(command, " \n");
    if (token == NULL)
        return;

    /* Tokenize the command and store tokens in an array */
    while (token != NULL) {
        tokens = realloc(tokens, sizeof(char *) * (token_count + 2));
        if (tokens == NULL) {
            perror("realloc");
            return;
        }
        tokens[token_count] = token;
        token_count++;
        token = strtok(NULL, " \n");
    }
    tokens[token_count] = NULL; /* Set the last element to NULL */

    /* Create the child process */
    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        free(tokens); /* Free allocated memory */
        return;
    }

    if (child_pid == 0) { /* Child process */
        /* Build the full path to the executable using custom directory structure */
        char *full_path = malloc(strlen("./my_bin/") + strlen(tokens[0]) + 1);
        if (full_path == NULL) {
            perror("malloc");
            free(tokens); /* Free allocated memory */
            exit(EXIT_FAILURE); /* Exit child process with failure status */
        }
        strcpy(full_path, "./my_bin/");
        strcat(full_path, tokens[0]);

        /* Execute the command using execve */
        if (execve(full_path, tokens, env) == -1) {
            perror("execve error");
            free(full_path);
            free(tokens); /* Free allocated memory */
            exit(EXIT_FAILURE); /* Exit child process with failure status */
        }
    } else { /* Parent process */
        /* Wait for the child process to complete */
        waitpid(child_pid, NULL, 0);
        free(tokens); /* Free allocated memory */
    }
}
