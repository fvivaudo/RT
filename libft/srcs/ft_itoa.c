/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adberard <adberard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/08 00:55:33 by adberard          #+#    #+#             */
/*   Updated: 2014/11/27 14:49:12 by adberard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static size_t	len(int n)
{
	size_t	i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char			*ft_itoa(int n)
{
	size_t			l;
	char			*s;
	unsigned int	tmp;

	l = len(n);
	tmp = n;
	if (n < 0)
	{
		tmp = -n;
		l++;
	}
	if (!(s = ft_strnew(l)))
		return (NULL);
	s[--l] = tmp % 10 + '0';
	while (tmp /= 10)
		s[--l] = tmp % 10 + '0';
	if (n < 0)
		s[0] = '-';
	return (s);
}
