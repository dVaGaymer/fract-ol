/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alopez-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/25 09:34:07 by alopez-g          #+#    #+#             */
/*   Updated: 2022/07/24 05:12:31 by alopez-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	long	aux;
	int		sign;

	aux = 0;
	sign = 1;
	while (*str == ' ' || *str == '\n' || *str == '\v' || *str == '\t'
		|| *str == '\r' || *str == '\f')
		str++;
	if (*str == 43 || *str == 45)
	{
		if (*str == 45)
			sign = -1;
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		aux = aux * 10 + (*str - 48);
		str++;
	}
	return ((int)(aux * sign));
}
