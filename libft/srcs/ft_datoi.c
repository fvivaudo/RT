/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_datoi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/05 09:12:45 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/12/18 17:30:53 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double		ft_datoi2(const char *str, double res)
{
	int		i;
	double	res2;
	int		pow;

	pow = 0;
	res2 = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res2 = res2 * 10 + str[i] - '0';
		++pow;
		++i;
	}
	while (pow > 0)
	{
		res2 /= 10;
		--pow;
	}
	res += res2;
	return (res);
}

double		ft_datoi(const char *str)
{
	int		i;
	double	res;
	int		neg;

	neg = 0;
	res = 0;
	i = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == ' ' ||
	str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-')
	{
		neg = 1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + str[i++] - '0';
	if (neg == 1)
		return ((ft_datoi2(str + i + 1, res)) * (-1));
	return (ft_datoi2(str + i + 1, res));
}
