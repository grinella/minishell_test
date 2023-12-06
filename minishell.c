/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaruso <ecaruso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:57:44 by grinella          #+#    #+#             */
/*   Updated: 2023/12/06 17:59:43 by ecaruso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include <stdio.h>

char	**find_path(char **env)
{
	int		i;
	//int		p;
	char	**path;
	char	*res;

	path = 0;
	res = 0;
	i = 0;
	//p = 0;
	while (env[i] != 0)
	{
		if (ft_strcmp(env[i], "PATH=") == 0)
			res = env[i];
		i++;
	}
	while (*res != '/')
		res++;
	path = ft_split(res, ':');
	// printf("\necco la path: %s\n", res);
	// while (path[p] != NULL)
	// {
	// 	printf("\n%s/", path[p]);
	// 	p++;
	// }
	return (path);
}

int	main(int argc, char **argv, char **env)
{
	exmain(argc, argv, env);
	find_path(env);
}
