/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mult_mat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 19:36:32 by momartin          #+#    #+#             */
/*   Updated: 2016/12/09 19:48:07 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		check_for_error(t_matrix *m_out, t_matrix *m1, t_matrix *m2)
{
	if (!m_out || !m1 || !m2)
		return (-1);
	else if (m1->rows != m2->rows || m1->rows != m_out->rows ||
		m1->cols != m2->cols || m1->cols != m_out->cols)
		return (-2);
	else
		return (0);
}

int				ft_mult_mat(t_matrix *m_out, t_matrix *m1, t_matrix *m2)
{
	int				err;
	unsigned int	row;
	unsigned int	col;
	unsigned int	k;
	float			val;

	if ((err = check_for_error(m_out, m1, m2)) < 0)
		return (err);
	col = 0;
	while (col++ <= m2->cols)
	{
		row = 0;
		while (row++ <= m1->rows)
		{
			val = 0.0;
			k = 0;
			while (k++ < m1->cols)
			{
				val += m1->data[(k - 1) * m1->rows + (row - 1)] *
					m2->data[(col - 1) * m2->rows + (k - 1)];
			}
			m_out->data[(col - 1) * m_out->rows + (row - 1)] = val;
		}
	}
	return (0);
}
