/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaruso <ecaruso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:57:44 by grinella          #+#    #+#             */
/*   Updated: 2024/01/05 01:35:00 by duzegbu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <stdio.h>

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

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
    t_mini *mini = initialize_mini(envp);

    while (1)
    {
        char *input = readline("shell>$: ");
        if (!input)
        {
            printf("\n");
            free(mini);
            exit(0);
        }

        if (*input)
        {
            add_history(input);
            parse_input(mini, input);
            execute_commands(mini);
        }

        free(input);
    }

    return 0;
}
