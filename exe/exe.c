/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grinella <grinella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 11:15:23 by grinella          #+#    #+#             */
/*   Updated: 2023/12/05 11:20:07 by grinella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe.h"

int	error(char *str, char *err)
{
	while (*str)
		write(2, str++, 1);
	if (err)
		while (*err)
			write(2, err++, 1);
	write(2, "\n", 1);
	return (1);
}

int	exec(char **args, int len, int fd, char **env)
{
	args[len] = NULL;
	dup2(fd, 0);
	close(fd);
	execve(args[0], args, env);
	return (error("error: cannot execute ", args[0]));
}

int	exmain(int argc, char **argv, char **env)
{
	int	i;
	int	pid;
	int	tmp;
	int	fd[2];

	i = 0;
	pid = 0;
	tmp = dup(0);
	(void)argc;
	while (argv[i] && argv[i + 1])
	{
		argv = &argv[i + 1];
		i = 0;
		while (argv[i] && strcmp(argv[i], "|"))
			i++;
		if (i != 0 && (argv[i] == NULL))
		{
			pid = fork();
			if (!pid)
			{
				if (exec(argv, i, tmp, env))
					return (1);
			}
			else
			{
				close(tmp);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				tmp = dup(0);
			}
		}
		else if (i != 0 && !strcmp(argv[i], "|"))
		{
			pipe(fd);
			pid = fork();
			if (!pid)
			{
				dup2(fd[1], 1);
				close(fd[1]);
				close(fd[0]);
				if (exec(argv, i, tmp, env))
					return (1);
			}
			else
			{
				close(tmp);
				close(fd[1]);
				tmp = fd[0];
			}
		}
	}
	close(tmp);
	return (0);
}
