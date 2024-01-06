/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaruso <ecaruso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:57:44 by grinella          #+#    #+#             */
/*   Updated: 2024/01/06 02:01:56 by duzegbu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

t_mini *initialize_mini(char **envp)
{
    t_mini *mini = malloc(sizeof(t_mini));
    if (!mini)
    {
        exit(EXIT_FAILURE);
    }

    mini->cmds = NULL;
    mini->cmds_count = 0;
    mini->fdin = 0;  
    mini->fdout = 1; 
    mini->env = envp;
    mini->toks = NULL;

    return mini;
}

void execute_commands(t_mini *mini) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execvp(mini->cmds->cmd, mini->cmds->args);
        perror("Execvp failed");
        fprintf(stderr, "Command: %s\n", mini->cmds->cmd);
        fprintf(stderr, "Arguments:\n");
        for (int i = 0; mini->cmds->args[i] != NULL; i++) {
            fprintf(stderr, "  %s\n", mini->cmds->args[i]);
        }

        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        } else {
            printf("Child process did not exit normally\n");
        }
    }
}

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

void free_cmd(t_cmds *cmd) {
    free(cmd->cmd);
    for (int i = 0; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd->redirect->infile);
    free(cmd->redirect->outfile);
    free(cmd->redirect);
    free(cmd);
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
    t_mini *mini = malloc(sizeof(t_mini));
    mini->cmds = malloc(sizeof(t_cmds));
    mini->cmds->redirect = malloc(sizeof(t_redirect));
    mini = initialize_mini(envp);

    while (1)
    {
        char *input = readline("shell% ");
        if (!input)
        {
            printf("\n");
            free_cmd(mini->cmds);
            exit(0);
        }
        
        add_history(input);
        parse_input(mini, input);
        execute_commands(mini);
        free(input);

    }

    return 0;
}
