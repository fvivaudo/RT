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

t_obj	addslice(t_obj obj, t_slice new)
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

t_obj	addneg(t_obj obj, t_neg new)
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

t_env	addlight(t_env e, t_light new)
{
	int i;

	i = 0;
	while (e.lights[i].set == TRUE && i < LIMIT_NEG)
	{
		++i;
	}
	if (i < LIMIT_NEG)
	{
		memcpy(&e.lights[i], &new, sizeof(t_light));
		e.lights[i].set = TRUE;
	}
	return (e);
}