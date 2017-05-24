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

typedef struct 	s_lstobj
{
	t_obj *temp;
	t_obj *current;
	t_obj *previous;
}				t_lstobj;

t_obj	*lstremoveoneobj(t_obj **alst, int id)
{
	t_lstobj o;

	if (id == ((*alst)->id))
	{
		o.temp = *alst;
		*alst = (*alst)->nextitem;
		o.temp->nextitem = NULL;
		return (o.temp);
	}
	o.current = (*alst)->nextitem;
	o.previous = *alst;
	while (o.current != NULL && o.previous != NULL)
	{
		if (id == o.current->id)
		{
			o.temp = o.current;
			o.previous->nextitem = o.current->nextitem;
			o.temp->nextitem = NULL;
			return (o.temp);
		}
		o.previous = o.current;
		o.current = o.current->nextitem;
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
