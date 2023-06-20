/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_gettimeofday.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 19:39:00 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/10 19:39:00 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

long	my_gettimeofday(void)
{
	struct timeval	tv;
	long			ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ret);
}
