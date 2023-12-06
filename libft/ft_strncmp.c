/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grinella <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 13:38:21 by grinella          #+#    #+#             */
/*   Updated: 2023/01/25 14:43:34 by grinella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(char *s1, char *s2, unsigned int n)
{
	unsigned int	e;

	if (n == 0)
		return (0);
	e = 0;
	while ((s1[e]) && (s2[e]) && (e < n - 1) && (s1[e] == s2[e]))
	{
		e++;
	}
	return ((unsigned char)s1[e] - (unsigned char)s2[e]);
}
