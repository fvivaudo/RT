/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 00:34:22 by momartin          #+#    #+#             */
/*   Updated: 2016/11/14 06:00:40 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list	*el;

	el = (t_list *)ft_memalloc(sizeof(t_list));
	if (!el)
		return (NULL);
	if (!content)
	{
		el->content = NULL;
		el->content_size = 0;
	}
	else
	{
		el->content = ft_memalloc(content_size);
		el->content = ft_memcpy(el->content, content, content_size);
		el->content_size = content_size;
	}
	el->next = NULL;
	return (el);
}
