#ifndef SIMPLE_SHELL
#define SIMPLE_SHELL

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv, char **env);
void execute(char *command, char **env);
char **get_paths(void);
void noninteractive_mode(char **env);
int is_empty_or_whitespace(const char *str);
void execute_with_args(char *full_path, char **args, char **env);

#endif

