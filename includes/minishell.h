/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaruso <ecaruso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 11:16:01 by grinella          #+#    #+#             */
/*   Updated: 2023/12/06 17:59:11 by ecaruso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include "../libft/libft.h"

typedef struct s_build
{
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}			t_build;

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	pid_t	pid;
}			t_prompt;

int	exmain(int argc, char **argv, char **env);
int	ft_strcmp(char *s1, char *s2);