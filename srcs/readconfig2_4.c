/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:58:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/05/24 13:56:15 by adberard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

char		**read_get_last_compose_line(char **line)
{
	while (!strstr(*line, "}"))
		line++;
	return (line);
}

void		read_get_composes(t_parser *r)
{
	char **line;

	line = LIN;
	while (*line)
	{
		if (strstr(*line, "compose"))
			read_get_compose_line(r, line, read_get_last_compose_line(line));
		line++;
	}
}

void		read_get_effect(t_parser *r, char *line)
{
	read_apply_one(r, line, "EFFECT ");
	fprintf(r->fi, "\n");
}

void		read_work(t_parser *r)
{
	LIN = ft_strsplitline(CON);
	read_get_composes(r);
	r->tmpfree = LIN;
	while (*LIN)
	{
		if (strstr(*LIN, "object") || read_is_anim(r, *LIN))
		{
			r->obj = LIN;
			read_set_last_line_obj(r);
			read_work_obj(r, LIN);
			read_get_slices(r);
			read_get_negatives(r);
			LIN = r->endobj;
			fprintf(r->fi, "\n");
		}
		if (strstr(*LIN, "effect"))
			read_get_effect(r, *LIN);
		LIN++;
	}
}

void		check_brackets_add(t_checker *c, char *str)
{
	if (str[c->i] == '{' || str[c->i] == '[' || str[c->i] == '(')
	{
		c->tab[c->itab] = str[c->i];
		c->itab++;
	}
}
