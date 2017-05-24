#include "../includes/rtv1.h"


typedef struct s_read
{
	int 		pos[3];
	int 		material[8];
	int 		lookat[3];
	int 		radius;
}				t_read;

# define CON r->content
# define LIN r->lines

typedef struct s_checker
{
	char 		tab[4096];
	int 		i;
	int 		itab;
}				t_checker;

typedef struct s_parser
{
	char 		*content;
	char 		**lines;
	char 		**obj;
	char 		**endobj;
	char 		**tmpfree;
	int 		comp_id;
	FILE 		*fi;
}				t_parser;

void check_quit()
{
	ft_putstr("fichier invalide\n");
	exit(0);
}

void free_tab(char **str)
{
	char **tmp;

	tmp = str;
	while (*str)
	{
		free(*str);
		str++;
	}
	free(tmp);
}

char *get_file_content(char *filename)
{
	char *r;
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	r = malloc(fsize + 1);
	fread(r, fsize, 1, f);
	fclose(f);
	r[fsize] = 0;
	return r;
}

char  **read_skip_list(t_parser *r, char **line)
{
	(void)r;
	if (strstr(*line, "negative") || strstr(*line, "slices"))
		while(!strstr(*line, "]"))
			line++;
	return (line);
}

char **read_get_one(t_parser *r, char **line ,char *str)
{
	while (line != r->endobj)
	{
		line = read_skip_list(r, line);
		if (strstr(*line, str))
			return line;
		line++;
	}
	return NULL;
}

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

void read_get_slices_set(t_parser *r, char **line)
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


void read_get_slices(t_parser *r)
{
	char **line;
	char **endobj;
	int ok;

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

void read_get_negatives(t_parser *r)
{
	char **line;
	char **endobj;
	int ok;

	ok = 0;
	line = r->obj;
	endobj = r->endobj;
	while (line != endobj)
	{
		if (ok)
		{
			if (strstr(*line, "object"))
			{
				fprintf(r->fi," NEGATIVE ");
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

void read_compose_apply_id(t_parser *r, char *line)
{
	read_apply_one(r, line, "");
	fprintf(r->fi," ID %d\nOBJECT %d", r->comp_id, r->comp_id);
	r->comp_id++;
}

void read_get_compose_line(t_parser *r, char **line, char **end)
{
	char **tmp;

	fprintf(r->fi,"\nCOMPOSE ");
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
	fprintf(r->fi,"\n");
}

char **read_get_last_compose_line(char **line)
{
	while (!strstr(*line, "}"))
		line++;
	return line;
}

void read_get_composes(t_parser *r)
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

void read_get_effect(t_parser *r, char *line)
{
	read_apply_one(r, line, "EFFECT ");
	fprintf(r->fi,"\n");
}

void read_work(t_parser *r)
{
	LIN = ft_strsplitline(CON);
	read_get_composes(r);
	r->tmpfree = LIN;
	while (*LIN)
	{
		if (strstr(*LIN,"object") || read_is_anim(r, *LIN))
		{
			r->obj = LIN;
			read_set_last_line_obj(r);
			read_work_obj(r, LIN);
			read_get_slices(r);
			read_get_negatives(r);
			LIN = r->endobj;
			fprintf(r->fi,"\n");
		}
		if (strstr(*LIN, "effect"))
			read_get_effect(r, *LIN);
		LIN++;
	}
}



void check_brackets_add(t_checker *c, char *str)
{
	if (str[c->i] == '{' || str[c->i] == '[' || str[c->i] == '(')
	{
		c->tab[c->itab] = str[c->i];
		c->itab++;
	}
}

void check_brackets_rem(t_checker *c, char *str)
{
	if ((str[c->i] == '}' || str[c->i] == ']' || str[c->i] == ')') &&
		c->itab <= 0)
		check_quit();
	if (str[c->i] == '}')
	{
		c->itab--;
		if (c->tab[c->itab] != '{')
			check_quit();
	}
	else if (str[c->i] == ']')
	{
		c->itab--;
		if (c->tab[c->itab] != '[')
			check_quit();
	}
	else if (str[c->i] == ')')
	{
		c->itab--;
		if (c->tab[c->itab] != '(')
			check_quit();
	}
}

int 	check_brackets(t_parser *r)
{
	t_checker c;
	c.i = 0;
	c.itab = 0;
	while (CON[c.i])
	{
		check_brackets_add(&c, CON);
		check_brackets_rem(&c, CON);
		c.i++;
	}
	if (c.itab != 0)
		check_quit();
	return (1);
}

void check_content_detail(char *line)
{
	if (!strstr(line, "type") && !strstr(line, "pos") &&
		!strstr(line, "orientation") && !strstr(line, "material") &&
		!strstr(line, "lookat") && !strstr(line, "object") &&
		!strstr(line, "{") && !strstr(line, "}") &&
		!strstr(line, "[") && !strstr(line, "]") &&
		!strstr(line, "radius") && !strstr(line, "negative") &&
		!strstr(line, "compose") && !strstr(line, "rotate") &&
		!strstr(line, "slices") && !strstr(line, "knife") &&
		!strstr(line, "id") && !strstr(line, "intensity") &&
		!strstr(line, "anim") && !strstr(line, "frame") &&
		!strstr(line, "style") && line[0] != 0 &&
		!strstr(line, "angle") && !strstr(line, "effect") &&
		!strstr(line, "torus"))
		check_quit();
}

int check_content(t_parser *r)
{
	char **lines;
	char **tmp;

	lines = ft_strsplitline(CON);
	tmp = lines;
	while (*lines)
	{
		check_content_detail(*lines);
		lines++;
	}
	free_tab(tmp);
	return (1);
}

void read_check(t_parser *r)
{
	if (!check_brackets(r))
		check_quit();
	if (!check_content(r))
		check_quit();
}

void				readConfig2(char *filename)
{
	t_parser r;

	r.comp_id = 1;
	r.fi = fopen("confage", "w");
	r.content = get_file_content(filename);
	read_check(&r);
	read_work(&r);
	free_tab(r.tmpfree);
	free(r.content);
	fclose(r.fi);
}
