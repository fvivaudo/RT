/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_new_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 15:22:47 by momartin          #+#    #+#             */
/*   Updated: 2016/12/14 22:25:37 by momartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_matrix	*ft_new_matrix(unsigned int rows, unsigned int cols)
{
	t_matrix	*mat;

	mat = (t_matrix*)ft_memalloc(sizeof(t_matrix));
	mat->rows = rows;
	mat->cols = cols;
	mat->data = (float *)ft_memalloc(sizeof(float) * rows * cols);
	return (mat);
}
