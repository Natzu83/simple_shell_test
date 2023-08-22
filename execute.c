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
            char *tokens[2];
            tokens[0] = full_path;
            tokens[1] = NULL;
            if (execve(full_path, tokens, env) == -1)
            {
                perror("execve error");
                exit(EXIT_FAILURE);
            }
        }
        else  /* Arguments provided */
        {
            int num_args = 0;
            char **args_tokens;

            while (args[num_args] != NULL)
                num_args++;

            args_tokens = malloc(sizeof(char *) * (num_args + 2));
            if (args_tokens == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            args_tokens[0] = full_path;
            for (num_args = 0; args[num_args] != NULL; num_args++)
                args_tokens[num_args + 1] = args[num_args];
            args_tokens[num_args + 1] = NULL;

            if (execve(full_path, args_tokens, env) == -1)
            {
                perror("execve error");
                exit(EXIT_FAILURE);
            }

            free(args_tokens);
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

    if (paths == NULL)
    {
        fprintf(stderr, "Error getting paths\n");
        return;
    }

    printf("Executing command: %s\n", command); /* Debug print */

    if (strchr(command, '/') != NULL || strchr(command, '-') != NULL)
    {
        char *token = strtok(command, " ");
        char **args = NULL;
        int num_args = 0;

        while (token != NULL)
        {
            args = realloc(args, sizeof(char *) * (num_args + 2));
            if (args == NULL)
            {
                perror("realloc");
                free(paths);
                return;
            }

            args[num_args] = token;
            num_args++;

            token = strtok(NULL, " ");
        }
        args[num_args] = NULL;

        execute_with_args(args[0], args, env);

        free(args);
        free(paths);
        return;
    }
    else
    {
        /* ... (Rest of the code, as before) */
    }

    fprintf(stderr, "Command not found\n");
    free(paths);
}
