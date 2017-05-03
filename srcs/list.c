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

//t_objconf	*RemoveNode(t_objconf **alst, int id)
t_objconf	*lstremoveoneobj(t_objconf **alst, int id)
{
	t_objconf *temp;
	t_objconf *current;
	t_objconf *previous;

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
t_objconf	*lstremoveoneobj(t_objconf **alst, int id)
{
	t_objconf	*prev;
	t_objconf	*next;
	t_objconf	*cursor;

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

void	lstaddobj(t_objconf **alst, t_objconf *new)
{
	if (alst && *alst && new)
	{
		new->nextitem = *alst;
		*alst = new;
	}
	else if (alst)
		*alst = new;
}

t_objconf	addslice(t_objconf obj, t_slice new)
{
	int i;

	i = 0;
	while (obj.nextslice[i].set == TRUE && i < LIMIT_SLICE)
	{
		++i;
	}
	if (i < LIMIT_SLICE)
	{
		obj.nextslice[i].pos = new.pos;
		obj.nextslice[i].dir = new.dir;
		obj.nextslice[i].set = TRUE;
	}
	return (obj);
}

t_objconf	addneg(t_objconf obj, t_neg new)
{
	int i;

	i = 0;
	while (obj.nextneg[i].set == TRUE && i < LIMIT_NEG)
	{
		++i;
	}
	if (i < LIMIT_NEG)
	{
		memcpy(&obj.nextneg[i], &new, sizeof(t_neg));
		obj.nextneg[i].pos = vectoradd(obj.pos, obj.nextneg[i].pos); // add something for direction too?
		obj.nextneg[i].set = TRUE;
	}
	return (obj);
}

void	addlight(t_light *lights, t_light new)
{
	int i;

	i = 0;
	while (lights[i].set == TRUE && i < LIMIT_LIGHT)
	{
		++i;
	}
	if (i < LIMIT_LIGHT)
	{
		memcpy(&lights[i], &new, sizeof(t_light));
		lights[i].set = TRUE;
	}
}