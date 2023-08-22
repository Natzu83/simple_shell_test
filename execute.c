#include "simple_shell.h"

/**
 * execute - Function to execute a command using fork and execve
 *
 * @command: The command to execute
 *
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
        if (execve(full_path, args, env) == -1)
        {
            perror("execve error");
            exit(EXIT_FAILURE);
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
