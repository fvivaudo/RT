#include "../includes/rtv1.h"

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
