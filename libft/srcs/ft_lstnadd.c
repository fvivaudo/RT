/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnadd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/12 09:57:43 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/12 10:15:05 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <string.h>

void	ft_lstnadd(t_list **alst, t_list *new, size_t n)
{
	t_list	*prev;
	t_list	*next;
	t_list	*cursor;

	next = NULL;
	if (alst && *alst)
	{
		cursor = *alst;
		prev = cursor;
		if (n == 0)
		{
			new->next = *alst;
			*alst = new;
		}
		while (cursor && n-- > 0)
		{
			prev = cursor;
			cursor = cursor->next;
			if (cursor)
				next = cursor->next;
		}
		prev->next = new;
		new->next = next;
	}
}
