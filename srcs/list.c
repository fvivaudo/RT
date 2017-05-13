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

//t_obj	*RemoveNode(t_obj **alst, int id)
t_obj	*lstremoveoneobj(t_obj **alst, int id)
{
	t_obj *temp;
	t_obj *current;
	t_obj *previous;

	if (id == ((*alst)->id))
	{
		temp = *alst;
		*alst = (*alst)->nextitem; //set nextchild to null too?
		temp->nextitem = NULL;
		return (temp);
	}
	current = (*alst)->nextitem;
	previous = *alst;
	while (current != NULL && previous != NULL)
	{
		if (id == current->id)
		{
			temp = current;
			previous->nextitem = current->nextitem;
			temp->nextitem = NULL;
			return (temp);
		}
		previous = current;
		current = current->nextitem;
	}
	return (NULL);
}
/*
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
	//	if (cursor->id == 10)
	//		printf("ok1\n");
		if (cursor->id == id)
		{
			(*alst = (*alst)->nextitem);
			cursor->nextitem = NULL;
			return (cursor);
		}
		while (cursor->nextitem && cursor->id != id)
		{
			prev = cursor;
			cursor = cursor->nextitem;
			if (cursor->nextitem)
			next = cursor->nextitem;
		}
	//	if (cursor->id == 10)
	//		printf("ok2\n");
		prev->nextitem = next;
		cursor->nextitem = NULL;
		return (cursor);
	}
	return (NULL);
}*/

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