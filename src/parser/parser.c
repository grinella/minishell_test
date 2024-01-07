#include "../../includes/minishell.h"

int open_next_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void handle_single_quote(char *token, t_mini *mini)
{
    if (!mini->cmds->args)
    {
        mini->cmds->args = malloc(2 * sizeof(char *));
        mini->cmds->args[0] = strdup(token);
        mini->cmds->args[1] = NULL;
    }
    else
    {
        int args_count = 0;
        while (mini->cmds->args[args_count] != NULL)
            args_count++;

        mini->cmds->args = realloc(mini->cmds->args, (args_count + 2) * sizeof(char *));
        if (!mini->cmds->args)
        {
            exit(EXIT_FAILURE);
        }
        mini->cmds->args[args_count] = strdup(token);
        mini->cmds->args[args_count + 1] = NULL;
    }
}

void handle_double_quote(char *token, t_mini *mini)
{
    if (!mini->cmds->args)
    {
        mini->cmds->args = malloc(2 * sizeof(char *));
        mini->cmds->args[0] = strdup(token);
        mini->cmds->args[1] = NULL;
    }
    else
    {
        int args_count = 0;
        while (mini->cmds->args[args_count] != NULL)
            args_count++;

        mini->cmds->args = realloc(mini->cmds->args, (args_count + 2) * sizeof(char *));
        if (!mini->cmds->args)
        {
            exit(EXIT_FAILURE);
        }
        mini->cmds->args[args_count] = strdup(token);
        mini->cmds->args[args_count + 1] = NULL;
    }
}

//redirection da cambiare perche non funziona un cazzo
void handle_redirection(char *token, t_mini *mini)
{
    if (strcmp(token, "<") == 0)
        mini->fdin = open_next_file(token);
    else if (strcmp(token, ">") == 0)
    {
        mini->fdout = open_next_file(token); 
    }
    else if (strcmp(token, ">>") == 0)
    {
        mini->fdout = open_next_file(token);
    }
}

void parse_input(t_mini *mini, char *input)
{
    char *token;
    char *saveptr;
    const char *delimiters = " \t\n";

    token = strtok_r(input, delimiters, &saveptr);
    while (token != NULL)
    {
        if (strchr(token, '\'') != NULL)
        {
            handle_single_quote(token, mini);
        }
        else if (strchr(token, '\"') != NULL)
        {
            handle_double_quote(token, mini);
        }
        else
        {
            printf("Token: %s\n", token);
        }
        handle_redirection(token, mini);
        token = strtok_r(NULL, delimiters, &saveptr);
    }
}