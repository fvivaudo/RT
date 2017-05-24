/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:58:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/05/24 13:50:25 by adberard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void		read_get_slices_set(t_parser *r, char **line)
{
	char **tmp;
	char **tmp2;

	tmp = line;
	tmp2 = r->endobj;
	while (!strstr(*line, "}"))
		line++;
	r->endobj = line;
	read_work_obj(r, tmp);
	r->endobj = tmp2;
}

void		read_get_slices(t_parser *r)
{
	char	**line;
	char	**endobj;
	int		ok;

	ok = 0;
	line = r->obj;
	endobj = r->endobj;
	while (line != endobj)
	{
		if (ok)
		{
			if (strstr(*line, "object"))
				read_get_slices_set(r, line);
			if (strstr(*line, "]"))
				return ;
		}
		if (strstr(*line, "slice"))
		{
			ok = 1;
		}
		line++;
	}
}

void		read_get_negatives(t_parser *r)
{
	char	**line;
	char	**endobj;
	int		ok;

	ok = 0;
	line = r->obj;
	endobj = r->endobj;
	while (line != endobj)
	{
		if (ok)
		{
			if (strstr(*line, "object"))
			{
				fprintf(r->fi, " NEGATIVE ");
				read_get_slices_set(r, line);
			}
			if (strstr(*line, "]"))
				return ;
		}
		if (strstr(*line, "negative"))
			ok = 1;
		line++;
	}
}

void		read_compose_apply_id(t_parser *r, char *line)
{
	read_apply_one(r, line, "");
	fprintf(r->fi, " ID %d\nOBJECT %d", r->comp_id, r->comp_id);
	r->comp_id++;
}

void		read_get_compose_line(t_parser *r, char **line, char **end)
{
	char **tmp;

	fprintf(r->fi, "\nCOMPOSE ");
	line++;
	tmp = line;
	while (line != end)
	{
		if (strstr(*line, "id"))
			read_compose_apply_id(r, *line);
		line++;
	}
	while (tmp != end)
	{
		if (strstr(*tmp, "pos"))
			read_apply_one(r, *tmp, " POSITION ");
		if (strstr(*tmp, "orientation"))
			read_apply_one(r, *tmp, " ORIENTATION ");
		if (strstr(*tmp, "rotate"))
			read_apply_one(r, *tmp, " ROTATE ");
		tmp++;
	}
	fprintf(r->fi, "\n");
}
