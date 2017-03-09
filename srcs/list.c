/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:57:51 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/01/09 13:39:27 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

/*
**		Simple removal, doesn't free anything
*/
t_obj	*lstremoveoneobj(t_obj **alst, int id)
{
	t_obj	*prev;
	t_obj	*next;
	t_obj	*cursor;

	next = NULL;
	if (alst && *alst)
	{
		cursor = *alst;
		prev = cursor;
		if (cursor->id == id)
			(*alst = (*alst)->nextitem);
		while (cursor->nextitem && cursor->id != id)
		{
			prev = cursor;
			cursor = cursor->nextitem;
			next = cursor->nextitem;
		}
		prev->nextitem = next;
		return (cursor);
	}
	return (NULL);
}

void	lstaddobj(t_obj **alst, t_obj *new)
{
	if (alst && *alst && new)
	{
		new->nextitem = *alst;
		*alst = new;
	}
	else if (alst)
		*alst = new;
}

//lstaddslice not used
void	lstaddslice(t_obj **alst, t_obj *new)
{
	if (alst && *alst && new)
	{
		new->nextslice = *alst;
		*alst = new;
	}
	else if (alst)
		*alst = new;
}

void	lstaddneg(t_obj **alst, t_obj *new)
{
	if (alst && *alst && new)
	{
		new->nextneg = *alst;
		*alst = new;
	}
	else if (alst)
		*alst = new;
}

void	lstaddlight(t_light **alst, t_light *new)
{
	if (alst && *alst && new)
	{
		new->next = *alst;
		*alst = new;
	}
	else if (alst)
		*alst = new;
}