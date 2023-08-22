#include "simple_shell.h"

/**
 * execute_with_args - Function to execute a command with arguments using fork and execve
 *
 * @full_path: The full path to the executable
 * @args: The arguments to the command (including the command itself)
 * @env: The environment variables
 */
void execute_with_args(char *full_path, char **args, char **env)
{
    pid_t child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork");
        return;
    }

    if (child_pid == 0)
    {
        if (args == NULL)  /* No arguments provided */
        {
            char *tokens[2] = {full_path, NULL};
            if (execve(full_path, tokens, env) == -1)
            {
                perror("execve error");
                exit(EXIT_FAILURE);
            }
        }
        else  /* Arguments provided */
        {
            /* Count the number of arguments */
            int num_args = 0;
            while (args[num_args] != NULL)
                num_args++;

            /* Construct the tokens array */
            char **tokens = malloc(sizeof(char *) * (num_args + 2));
            if (tokens == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            tokens[0] = full_path;
            for (int i = 0; i < num_args; i++)
                tokens[i + 1] = args[i];
            tokens[num_args + 1] = NULL;

            if (execve(full_path, tokens, env) == -1)
            {
                perror("execve error");
                exit(EXIT_FAILURE);
            }

            free(tokens);
        }
    }
    else
    {
        waitpid(child_pid, NULL, 0);
    }
}

void execute(char *command, char **env)
{
    char **paths = get_paths();
    int i;

    if (paths == NULL)
    {
        fprintf(stderr, "Error getting paths\n");
        return;
    }

    printf("Executing command: %s\n", command); /* Debug print */

    if (strchr(command, '/') != NULL || strchr(command, '-') != NULL)
    {
        execute_with_args(command, NULL, env);
        free(paths);
        return;
    }
    else
    {
        for (i = 0; paths[i] != NULL; i++)
        {
            char *full_path1;

            full_path1 = malloc(strlen(paths[i]) + strlen(command) + 2);
            if (full_path1 == NULL)
            {
                perror("malloc");
                free(paths);
                return;
            }
            sprintf(full_path1, "%s/%s", paths[i], command);

            printf("Full path 1: %s\n", full_path1); /* Debug print */

            if (access(full_path1, X_OK) == 0)
            {
                execute_with_args(full_path1, NULL, env);

                free(full_path1);
                free(paths);
                return;
            }

            free(full_path1);
        }
    }

    fprintf(stderr, "Command not found\n");
    free(paths);
}
