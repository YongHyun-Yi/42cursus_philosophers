/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:36:26 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/29 16:27:33 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

int	ft_atoi(const char *str)
{
	long long			sign;
	unsigned long long	result;

	while ((*str >= 9 && *str <= 13) || (*str == 32))
		str++;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		result *= 10;
		result += (*str - '0');
		str++;
		if (result > (unsigned long long)LONG_MAX + (sign < 0))
			return (-1 * (sign > 0));
	}
	return (result * sign);
}
