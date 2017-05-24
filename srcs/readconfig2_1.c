/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/31 15:58:09 by fvivaudo          #+#    #+#             */
/*   Updated: 2017/05/24 12:46:05 by adberard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rtv1.h>

void		check_quit(void)
{
	ft_putstr("fichier invalide\n");
	exit(0);
}

void		free_tab(char **str)
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

char		*get_file_content(char *filename)
{
	char	*r;
	long	fsize;
	FILE	*f;

	f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	r = malloc(fsize + 1);
	fread(r, fsize, 1, f);
	fclose(f);
	r[fsize] = 0;
	return (r);
}

char		**read_skip_list(t_parser *r, char **line)
{
	(void)r;
	if (strstr(*line, "negative") || strstr(*line, "slices"))
		while (!strstr(*line, "]"))
			line++;
	return (line);
}

char		**read_get_one(t_parser *r, char **line, char *str)
{
	while (line != r->endobj)
	{
		line = read_skip_list(r, line);
		if (strstr(*line, str))
			return (line);
		line++;
	}
	return (NULL);
}
