/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_transpose_mat.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 17:21:55 by momartin          #+#    #+#             */
/*   Updated: 2016/11/15 20:20:35 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_transpose_mat(t_matrix *m_out, t_matrix *m_in)
{
	unsigned int row;
	unsigned int col;

	if (!m_out || !m_in)
		return (-1);
	if (m_in->rows != m_out->cols || m_in->cols != m_out->rows)
		return (-2);
	row = 1;
	while (row <= m_in->rows)
	{
		col = 1;
		while (col <= m_in->cols)
		{
			m_out->data[(col - 1) * m_out->rows + (row - 1)] =
				m_in->data[(row - 1) * m_in->rows + (col - 1)];
			col++;
		}
		row++;
	}
	return (0);
}
