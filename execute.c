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
    char **args_tokens;
    pid_t child_pid;
    int i;

    args_tokens = NULL;

    child_pid = fork();
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

            while (args[num_args] != NULL)
                num_args++;

            args_tokens = malloc(sizeof(char *) * (num_args + 2));
            if (args_tokens == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            args_tokens[0] = full_path;
            for (i = 0; args[i] != NULL; i++)
                args_tokens[i + 1] = args[i];
            args_tokens[i + 1] = NULL;

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
    char ** paths;
    int num_args = 0;
    int i;

    char **args = malloc(sizeof(char *) * 2);
    args[0] = strtok(command, " ");

    while (args[num_args] != NULL)
    {
        num_args++;
        args[num_args] = strtok(NULL, " ");
    }

    paths = get_paths();

    if (paths == NULL)
    {
        fprintf(stderr, "Error getting paths\n");
        free(args);
        return;
    }

    printf("Executing command: %s\n", command); /* Debug print */

    args = realloc(args, sizeof(char *) * (num_args + 1));
    args[num_args] = NULL;

    if (strchr(args[0], '/') != NULL || strchr(args[0], '-') != NULL)
    {
        execute_with_args(args[0], args, env);
    }
    else
    {
        for (i = 0; paths[i] != NULL; i++)
        {
            char *full_path1;

            full_path1 = malloc(strlen(paths[i]) + strlen(args[0]) + 2);
            if (full_path1 == NULL)
            {
                perror("malloc");
                free(paths);
                free(args);
                return;
            }
            sprintf(full_path1, "%s/%s", paths[i], args[0]);

            printf("Full path 1: %s\n", full_path1); /* Debug print */

            if (access(full_path1, X_OK) == 0)
            {
                execute_with_args(full_path1, args, env);

                free(full_path1);
                free(paths);
                free(args);
                return;
            }

            free(full_path1);
        }

        fprintf(stderr, "Command not found\n");
    }

    free(paths);
    free(args);
}
