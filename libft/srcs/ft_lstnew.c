/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/10 12:30:35 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/29 12:55:03 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <string.h>

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*elem;

	elem = (t_list*)malloc(sizeof(t_list));
	if (!content)
	{
		elem->content_size = 0;
		elem->content = NULL;
	}
	else
	{
		elem->content_size = content_size;
		elem->content = ft_memalloc(content_size + 1);
		ft_memcpy(elem->content, content, content_size);
	}
	elem->next = NULL;
	return (elem);
}
