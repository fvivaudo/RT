/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/06 10:52:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/10 10:14:58 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

void	ft_strtrim2(char const *s, int *i, int *start, int *end)
{
	while ((s[*i] == ' ' || s[*i] == '\t' || s[*i] == '\n') && s[*i] != '\0')
	{
		++(*start);
		++(*i);
	}
	while (s[*i] != '\0')
		++(*i);
	*end = *i;
	if (s[*start] != '\0')
		while (s[*i - 1] == ' ' || s[*i - 1] == '\t' || s[*i - 1] == '\n')
		{
			--(*end);
			--(*i);
		}
}

char	*ft_strtrim(char const *s)
{
	char	*res;
	int		i;
	int		start;
	int		end;

	if (!s)
		return (NULL);
	start = 0;
	i = 0;
	ft_strtrim2(s, &i, &start, &end);
	res = (char*)malloc(end - start + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (start != end)
		res[i++] = s[start++];
	res[i] = '\0';
	return (res);
}
