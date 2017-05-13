/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplitline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/15 14:24:13 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/15 15:05:19 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

static	char	*ft_filling(int *i, char *res, const char *s)
{
	int z;

	z = 0;
	while (s[*i] != '\n' && s[*i] != 0)
	{
		res[z] = s[*i];
		*i = *i + 1;
		z++;
	}
	res[z] = 0;
	return (res);
}

static	int		ft_count_lines(const char *s)
{
	int count;
	int i;

	i = 0;
	count = 1;
	while (s[i] != 0)
		if (s[i++] == '\n')
			count++;
	return (count);
}

char			**ft_malloc(int *i, int *y, char const *s)
{
	char	**res;

	*i = 0;
	*y = 0;
	if (!s)
		return (NULL);
	res = (char**)malloc((ft_count_lines(s) + 1) * sizeof(char*) + 1);
	if (res == NULL)
		return (NULL);
	return (res);
}

char			**ft_strsplitline(char const *s)
{
	char	**res;
	int		i;
	int		y;
	int		size;

	if (!(res = ft_malloc(&i, &y, s)))
		return (NULL);
	while (s[i] != 0)
	{
		size = 0;
		while (s[i] != '\n' && s[i] != 0)
		{
			++size;
			++i;
		}
		i = i - size;
		if ((res[y] = (char*)malloc(size + 1)) == NULL)
			return (NULL);
		res[y] = ft_filling(&i, res[y], s);
		++y;
		++i;
	}
	res[y] = 0;
	return (res);
}
