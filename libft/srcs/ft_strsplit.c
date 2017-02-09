/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/06 11:35:41 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/10 10:15:07 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

static	void	ft_skip_spaces(int *i, char c, const char *s)
{
	while (s[*i] == c)
		*i = *i + 1;
}

static	char	*ft_filling(int *i, char *res, const char *s, char c)
{
	int z;

	z = 0;
	while (s[*i] != c && s[*i] != 0)
	{
		res[z] = s[*i];
		*i = *i + 1;
		z++;
	}
	res[z] = 0;
	return (res);
}

static	int		ft_count_words(const char *s, char c)
{
	int count;
	int i;

	i = 0;
	count = 0;
	while (s[i] != 0)
	{
		while (s[i] == c)
			i++;
		while (s[i] != c && s[i] != 0)
			i++;
		if (s[i] != 0 || count == 0)
			count++;
	}
	return (count);
}

char			**ft_resmalloc(int *i, int *y, char c, char const *s)
{
	char	**res;

	i = 0;
	y = 0;
	if (!s)
		return (NULL);
	res = (char**)malloc((ft_count_words(s, c) + 1) * sizeof(char*) + 1);
	if (res == NULL)
		return (NULL);
	ft_skip_spaces(i, c, s);
	return (res);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**res;
	int		i;
	int		y;
	int		size;

	if (!(res = ft_resmalloc(&i, &y, c, s)))
		return (NULL);
	while (s[i] != 0)
	{
		size = 0;
		while (s[i] != c && s[i] != 0)
		{
			++size;
			++i;
		}
		i = i - size;
		if ((res[y] = (char*)malloc(size + 1)) == NULL)
			return (NULL);
		res[y] = ft_filling(&i, res[y], s, c);
		++y;
		ft_skip_spaces(&i, c, s);
	}
	res[y] = 0;
	return (res);
}
