/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 04:01:17 by momartin          #+#    #+#             */
/*   Updated: 2016/11/14 04:03:40 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_lstlen(t_list *lst)
{
	t_list *tmp;
	size_t len;

	len = 0;
	if (lst)
	{
		tmp = lst;
		while (tmp)
		{
			len++;
			tmp = tmp->next;
		}
	}
	return (len);
}
