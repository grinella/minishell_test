#include "../../includes/minishell.h"

void handle_single_quote(char *token, t_mini *mini)
{
    if (!mini->cmds->args)
    {
        mini->cmds->args = malloc(sizeof(char *));
        mini->cmds->args[0] = NULL;
    }
    mini->cmds->args = realloc(mini->cmds->args, (strlen(token) + 1) * sizeof(char *));
    if (!mini->cmds->args)
    {
        exit(EXIT_FAILURE);
    }
    mini->cmds->args[mini->cmds->cmds_count] = strdup(token);
    mini->cmds->args[mini->cmds->cmds_count + 1] = NULL;

    mini->cmds->cmds_count++;
}

void handle_double_quote(char *token, t_mini *mini)
{
    if (!mini->cmds->args)
    {
        mini->cmds->args = malloc(sizeof(char *));
        mini->cmds->args[0] = NULL;
    }
    mini->cmds->args = realloc(mini->cmds->args, (strlen(token) + 1) * sizeof(char *));
    if (!mini->cmds->args)
    {
        exit(EXIT_FAILURE);
    }
    mini->cmds->args[mini->cmds->cmds_count] = strdup(token);
    mini->cmds->args[mini->cmds->cmds_count + 1] = NULL;

    mini->cmds->cmds_count++;
}

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
            handle_single_quote(token);
        }
        else if (strchr(token, '\"') != NULL)
        {
            handle_double_quote(token);
        }
        else
        {
            printf("Token: %s\n", token);
        }
        handle_redirection(token, mini);
        token = strtok_r(NULL, delimiters, &saveptr);
    }
}