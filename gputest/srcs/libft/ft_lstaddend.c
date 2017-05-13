/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstaddend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/14 19:32:19 by momartin          #+#    #+#             */
/*   Updated: 2016/12/14 19:45:32 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstaddend(t_list **alst, t_list *new)
{
	t_list *current;

	if (!&*alst || !new)
		return ;
	if (*alst && alst)
	{
		current = *alst;
		while (current->next)
			current = current->next;
		new->next = NULL;
		current->next = new;
	}
	else
		*alst = new;
}
