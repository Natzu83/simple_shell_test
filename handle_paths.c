#include "simple_shell.h"

/**
 * get_paths - Extracts paths from the PATH environment variable
 *
 * Return: A pointer to an array of strings containing the paths
 *         NULL on failure or if PATH is not found
 */
char **get_paths(void)
{
    char *path_env = getenv("PATH");
    char *path_copy;
    char *token;
    int num_paths = 0;
    char **paths = NULL;

    if (path_env == NULL)
    {
        perror("error ");
        return NULL;
    }

    path_copy = strdup(path_env);
    if (path_copy == NULL)
    {
        perror("error ");
        return NULL;
    }

    token = strtok(path_copy, ":");
    while (token != NULL)
    {
        paths = realloc(paths, sizeof(char *) * (num_paths + 1));
        if (paths == NULL)
        {
            perror("error ");
            free(path_copy);
            free(paths);
            return NULL;
        }
        paths[num_paths] = strdup(token);
        num_paths++;
        token = strtok(NULL, ":");
    }
    paths = realloc(paths, sizeof(char *) * (num_paths + 1));
    paths[num_paths] = NULL;

    free(path_copy);
    return paths;
}
