/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/09 14:46:31 by momartin          #+#    #+#             */
/*   Updated: 2016/11/12 23:48:20 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	ft_count_wrds(char *s, char c)
{
	size_t i;
	size_t n;

	i = 0;
	n = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			n++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (n);
}

static char		*ft_extract_wrd(char *s, char c)
{
	size_t	i;
	size_t	wrdlen;
	char	*str_out;

	i = 0;
	wrdlen = 0;
	while (s[wrdlen] && s[wrdlen] != c)
		wrdlen++;
	str_out = ft_strnew(sizeof(char) * wrdlen);
	if (!str_out)
		return (NULL);
	while (i < wrdlen)
	{
		str_out[i] = s[i];
		i++;
	}
	return (str_out);
}

char			**ft_strsplit(char const *s, char c)
{
	size_t	nbr_wrds;
	size_t	i;
	char	**tab;
	char	*str;

	if (s)
	{
		str = (char*)s;
		nbr_wrds = 0;
		i = 0;
		nbr_wrds = ft_count_wrds(str, c);
		if (!(tab = (char **)ft_memalloc(sizeof(char *) * (nbr_wrds + 1))))
			return (NULL);
		while (nbr_wrds--)
		{
			while (*str == c)
				str++;
			tab[i] = ft_extract_wrd(str, c);
			str += ft_strlen(tab[i]);
			i++;
		}
		return (tab);
	}
	return (NULL);
}
