/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/12/05 17:20:17 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/12/05 17:23:55 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_lstrev(t_list **alst, size_t size)
{
	t_list	*start1;
	t_list	*start2;
	t_list	*end;
	size_t	s;

	s = size;
	end = *alst;
	while (end->next)
		end = end->next;
	start1 = end;
	while (size-- != 0)
	{
		start2 = (*alst);
		s = size;
		while (s-- > 1)
			start2 = start2->next;
		start1->next = start2;
		start2->next = NULL;
		start1 = start2;
	}
	*alst = end;
}
