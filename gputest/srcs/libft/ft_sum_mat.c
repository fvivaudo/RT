/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sum_mat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 18:53:07 by momartin          #+#    #+#             */
/*   Updated: 2016/11/15 19:35:01 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_sum_mat(t_matrix *m_out, t_matrix *m1, t_matrix *m2)
{
	unsigned int row;
	unsigned int col;

	if (!m_out || !m1 || !m2)
		return (-1);
	if (m1->rows != m2->rows || m1->rows != m_out->rows ||
		m1->cols != m2->cols || m1->cols != m_out->cols)
		return (-2);
	col = 1;
	while (col <= m1->cols)
	{
		row = 1;
		while (row <= m1->rows)
		{
			m_out->data[(col - 1) * m_out->rows + (row - 1)] =
				m1->data[(col - 1) * m1->rows + (row - 1)] +
				m2->data[(col - 1) * m2->rows + (row - 1)];
			row++;
		}
		col++;
	}
	return (0);
}
