#include "simple_shell.h"

/**
 * noninteractive_mode - Handles the non-interactive mode of the shell
 *
 * @env: The environment variables
 */
void noninteractive_mode(char **env)
{
	const size_t BUFFER_SIZE = 1024;
    char *input_buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    ssize_t read;
	size_t input_size = BUFFER_SIZE;

	if (input_buffer == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

    printf("Non-Interactive Mode\n");

    while ((read = getline(&input_buffer, &input_size, stdin)) != -1) {
        /* Remove newline character if present */
        if (read > 0 && input_buffer[read - 1] == '\n')
            input_buffer[read - 1] = '\0';

        /* Exit loop if the user wants to exit */
        if (strcmp(input_buffer, "exit") == 0)
            break;

        /* Skip processing for empty or whitespace lines */
        if (is_empty_or_whitespace(input_buffer))
            continue;

        /* Execute the command from the input line */
        execute(input_buffer, env);
    }
	free(input_buffer);
}
