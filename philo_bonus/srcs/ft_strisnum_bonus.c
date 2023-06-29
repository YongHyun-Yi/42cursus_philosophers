/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strisnum_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:33:07 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/29 16:27:09 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

int	ft_strisnum(char *str)
{
	str += (*str == '+' || *str == '-');
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}
