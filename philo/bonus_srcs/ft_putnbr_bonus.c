/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:42:06 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/20 12:27:43 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

static void	recursion(long long long_n, int fd)
{
	if (!long_n)
		return ;
	ft_putnbr_fd(long_n / 10, fd);
	ft_putchar_fd((long_n % 10) + '0', fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	long long	long_n;

	long_n = (long long)n;
	if (long_n < 0)
	{
		ft_putchar_fd('-', fd);
		long_n *= -1;
	}
	if (long_n < 10)
		ft_putchar_fd(long_n + '0', fd);
	else
		recursion(long_n, fd);
}
