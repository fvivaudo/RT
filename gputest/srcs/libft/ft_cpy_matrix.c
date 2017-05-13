/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cpy_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: momartin <momartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/15 16:11:05 by momartin          #+#    #+#             */
/*   Updated: 2016/12/18 14:35:55 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_matrix	*ft_cpy_matrix(t_matrix *m)
{
	t_matrix *cpy_m;

	if (!m)
		return (NULL);
	cpy_m = ft_new_matrix(m->rows, m->cols);
	ft_memcpy(cpy_m->data, m->data, sizeof(float) * m->rows * m->cols);
	return (cpy_m);
}
