#include "simple_shell.h"

/**
 * is_empty_or_whitespace -Function checks if a string is empty or contains only whitespace
 *
 * @str: The input string
 *
 * Return: 1 if the string is empty or contains only whitespace, 0 otherwise
 */

int is_empty_or_whitespace(const char *str) {
    while (*str) {
        if (!isspace((unsigned char)*str))
            return (0);
        str++;
    }
    return (1);
}
