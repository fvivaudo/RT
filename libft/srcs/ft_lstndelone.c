/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstndelone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 17:29:19 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/12 09:54:57 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <string.h>
#include <stdlib.h>

void	ft_lstndelone(t_list **alst, size_t n, void (*del)(void*, size_t))
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
			(*alst = (*alst)->next);
		while (cursor->next && n-- > 0)
		{
			prev = cursor;
			cursor = cursor->next;
			next = cursor->next;
		}
		del(cursor->content, cursor->content_size);
		free(cursor);
		cursor = NULL;
		prev->next = next;
	}
}
