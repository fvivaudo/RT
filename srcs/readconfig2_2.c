/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:58:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/03/16 10:57:12 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void read_apply_one(t_parser *r, char *line, char *str)
{
	int ok;

	ok = 0;
	fprintf(r->fi,"%s", str);
	while (*line)
	{
		if (ok)
		{
			if (*line == ')')
			{
				fprintf(r->fi," ");
				return ;
			}
			if (*line == ',')
				fprintf(r->fi," ");
			else
				fprintf(r->fi,"%c", toupper(*line));
		}
		if (*line == '(')
			ok = 1;
		line++;
	}
	check_quit();
}

void read_work_obj2(t_parser *r, char **line)
{
	char **tmp;

	if ((tmp = read_get_one(r, line, "radius")))
		read_apply_one(r, *tmp, " RADIUS ");
	if ((tmp = read_get_one(r, line, "knife")))
		read_apply_one(r, *tmp, " ");
	if ((tmp = read_get_one(r, line, "angle")))
		read_apply_one(r, *tmp, " ANGLE ");
}

void read_work_obj(t_parser *r, char **line)
{
	char **tmp;
	if ((tmp = read_get_one(r, line, "style")))
		read_apply_one(r, *tmp, "");
	if ((tmp = read_get_one(r, line, "frame")))
		read_apply_one(r, *tmp, "");
	if ((tmp = read_get_one(r, line, "type")))
		read_apply_one(r, *tmp, "");
	if ((tmp = read_get_one(r, line, "pos")))
		read_apply_one(r, *tmp, "");
	if ((tmp = read_get_one(r, line, "orientation")))
		read_apply_one(r, *tmp, " ORIENTATION ");
	if ((tmp = read_get_one(r, line, "lookat")))
		read_apply_one(r, *tmp, " LOOKAT ");
	if ((tmp = read_get_one(r, line, "id")))
		read_apply_one(r, *tmp, " ID ");
	if ((tmp = read_get_one(r, line, "intensity")))
		read_apply_one(r, *tmp, " INTENSITY ");
	if ((tmp = read_get_one(r, line, "material")))
		read_apply_one(r, *tmp, " MATERIAL ");
	read_work_obj2(r, line);
}

int read_is_anim(t_parser *r, char *line)
{
	if (strstr(line, "anim"))
	{
		fprintf(r->fi,"ANIM ");
		return (1);
	}
	return (0);
}

void read_set_last_line_obj(t_parser *r)
{
	r->endobj = LIN;
	while (*r->endobj[0] != '}')
		r->endobj++;
}
