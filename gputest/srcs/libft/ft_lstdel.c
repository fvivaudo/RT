/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 01:56:10 by momartin          #+#    #+#             */
/*   Updated: 2016/11/14 04:26:52 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_lstdel(t_list **alst, void (*del)(void*, size_t))
{
	t_list *nxt;
	t_list *cur;

	if (!&*alst)
		return ;
	if (*alst && alst)
	{
		cur = *alst;
		while (cur != NULL)
		{
			nxt = cur->next;
			del(cur->content, cur->content_size);
			free(cur);
			cur = nxt;
		}
		*alst = NULL;
	}
}
