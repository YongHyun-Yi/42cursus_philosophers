/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 10:56:28 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/29 16:27:30 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

static int	malloc_size(long long long_n)
{
	int	size;

	if (long_n < 0)
		long_n *= -1;
	size = 0;
	while (long_n)
	{
		size++;
		long_n /= 10;
	}
	return (size);
}

static void	*to_ascii(long long long_n, int size, char *result)
{
	int	index;

	index = size;
	result[index--] = '\0';
	if (long_n < 0)
	{
		result[0] = '-';
		long_n *= -1;
		size--;
	}
	while (size--)
	{
		result[index--] = (long_n % 10) + '0';
		long_n /= 10;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	char		*result;
	int			size;
	long long	long_n;

	if (n == 0)
		return (ft_strdup("0"));
	long_n = (long long)n;
	size = malloc_size(long_n) + (long_n < 0);
	result = (char *)malloc(sizeof(char) * (size + 1));
	if (!result)
		return (0);
	return (to_ascii(long_n, size, result));
}
