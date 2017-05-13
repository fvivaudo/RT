/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_mat_elem.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 17:01:57 by momartin          #+#    #+#             */
/*   Updated: 2016/12/14 22:24:59 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_get_mat_elem(t_matrix *m, unsigned int row, unsigned int col,
						float *val)
{
	if (!m)
		return (-1);
	if (row > m->rows || col > m->cols)
		return (-2);
	*val = m->data[(col - 1) * m->rows + (row - 1)];
	return (0);
}
