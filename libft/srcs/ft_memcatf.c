/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcatf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/06 13:16:20 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/06 13:25:52 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_memcatf(const void *src, void *dst, int n, int dst_s)
{
	int end;
	int i;

	i = dst_s;
	end = n;
	while (i >= 0)
	{
		((char *)dst)[end + i] = ((char *)dst)[i];
		--i;
	}
	while (n-- > 0)
	{
		i++;
		((char *)dst)[i] = ((const char *)src)[i];
	}
	return (dst);
}
