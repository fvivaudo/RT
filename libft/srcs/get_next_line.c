/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 16:04:49 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/19 09:46:20 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <get_next_line.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static	int		ft_checkleft(char *left, int i)
{
	while (left[i] != '\n' && left[i] != '\0')
		++i;
	if (left[i] == '\n')
		return (i);
	return (-1);
}

static	char	*rec_line(int const fd, int *size, char **left, int *ret)
{
	char	buffer[BUFF_SIZE + 1];
	char	*line;
	int		i;

	line = 0;
	i = 0;
	if ((*ret = read(fd, buffer, BUFF_SIZE)) > 0)
	{
		buffer[*ret] = '\0';
		while (i < *ret && buffer[i] != '\n')
			++i;
		(*size) += i;
		if (*ret > i)
		{
			ft_strcpy(*left, buffer + i + 1);
			line = ft_strncpy(ft_strnew(*size), buffer, i);
		}
		else
		{
			line = rec_line(fd, size, left, ret);
			line = !line ? ft_strncpy(ft_strnew(*size), buffer, i) :
			ft_strcatf(buffer, line);
		}
	}
	return (line);
}

static	char	*rec_left(int const fd, int *size, char **left, int *ret)
{
	char	*line;
	char	leftb[BUFF_SIZE];

	(*size) += ft_strlen(*left);
	ft_strcpy(leftb, *left);
	line = rec_line(fd, size, left, ret);
	if (line == NULL)
		line = ft_strdup(leftb);
	else
		line = ft_strcatf(leftb, line);
	return (line);
}

static	int		rec_begin(int const fd, char **left, char **str, int size)
{
	int				i;
	int				ret;

	if ((i = ft_checkleft(*left, 0)) > -1)
	{
		(*left)[i] = '\0';
		*str = ft_strdup(*left);
		*left = ft_strcpy(*left, *left + i + 1);
		return (1);
	}
	*str = rec_left(fd, &size, left, &ret);
	if ((*left)[0] == 0 && ret == 0)
		return (0);
	return (ret);
}

int				get_next_line(int const fd, char **line)
{
	static	char	*leftb[7500] = {0};
	int				ret;
	char			*left;

	if ((unsigned const)fd > 7500)
		return (-1);
	ret = 0;
	if (!leftb[fd])
		leftb[fd] = ft_strnew(BUFF_SIZE);
	left = leftb[fd];
	ret = rec_begin(fd, &left, line, 0);
	if (ret > 0)
		return (1);
	if (ret == -1)
		return (-1);
	return (0);
}
