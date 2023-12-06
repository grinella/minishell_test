/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grinella <grinella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:18:52 by grinella          #+#    #+#             */
/*   Updated: 2023/01/30 14:46:27 by grinella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	size_t	e;

	i = 0;
	if (*s2 == '\0' || s2 == NULL)
		return ((char *)s1);
	while (i < n && s1[i] != '\0')
	{
		e = 0;
		while (i + e < n && s1[i + e] == s2[e] && s2[e] != '\0')
			e++;
		if (s2[e] == '\0')
			return ((char *)s1 + i);
		i++;
	}
	return (NULL);
}
