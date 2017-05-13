/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 03:45:09 by momartin          #+#    #+#             */
/*   Updated: 2016/11/14 04:28:40 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *new_lst;
	t_list *nxt;

	if (lst)
	{
		new_lst = (t_list *)ft_memalloc(sizeof(t_list) * ft_lstlen(lst));
		if (!new_lst)
			return (NULL);
		new_lst = f(ft_lstnew(lst->content, lst->content_size));
		nxt = new_lst;
		lst = lst->next;
		while (lst)
		{
			nxt->next = f(ft_lstnew(lst->content, lst->content_size));
			nxt = nxt->next;
			lst = lst->next;
		}
		return (new_lst);
	}
	return (NULL);
}
