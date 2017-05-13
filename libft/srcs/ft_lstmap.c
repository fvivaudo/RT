/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 14:22:40 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/11 15:56:30 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <string.h>

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *res;
	t_list *cursor1;

	res = NULL;
	if (lst && f)
	{
		res = f(ft_lstnew(lst->content, lst->content_size));
		cursor1 = res;
		lst = lst->next;
		while (lst != NULL)
		{
			cursor1->next = f(ft_lstnew(lst->content, lst->content_size));
			cursor1 = cursor1->next;
			lst = lst->next;
		}
	}
	return (res);
}
