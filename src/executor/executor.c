/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecaruso <ecaruso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:47:33 by ecaruso           #+#    #+#             */
/*   Updated: 2024/01/08 21:57:17 by ecaruso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *find_path(t_mini *mini, char **env)
{
	struct stat buff;
	int		i;
	char	**base;
	char	*temp;

	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	base = ft_split((env[i] + 5), ':');
	i = -1;
	while(base && base[++i])
	{
		temp = ft_strjoin(base[i], "/");
		printf("ciao:%s\n", temp);
		temp = ft_strjoin(temp, mini->str);
		if (!lstat(temp, &buff))
		{
			printf("%s\n",(temp));
			return(temp);
		}
		free(temp);
	}
	free(temp);
	return(temp);
}

void execute_commands(t_mini *mini, char **env)
{
	int		status;
	char	*path;
	pid_t	pid;

	path = find_path(mini, env);
	pid = fork();
	if (pid == -1)
	{
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
	else if (pid == 0)
	{
		execve(path, &mini->str, env);
		perror("Execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			printf("PORCODIO %d\n", WEXITSTATUS(status));
		else
			printf("PORCODIO\n");
	}
}
