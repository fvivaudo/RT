/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstget.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/11 17:17:53 by fvivaudo          #+#    #+#             */
/*   Updated: 2014/11/12 09:39:05 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <string.h>

t_list	*ft_lstget(t_list *alst, size_t n)
{
	if (alst)
		while (alst->next && n-- > 0)
			alst = alst->next;
	return (alst);
}
