/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 16:50:07 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/28 18:47:46 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_lstaddb(t_list **alst, t_list *new)
{
	t_list *cursor;

	if (alst && *alst && (*alst)->content)
	{
		cursor = (*alst);
		while (cursor->next)
			cursor = cursor->next;
		cursor->next = new;
	}
	else if (alst)
		*alst = new;
}
