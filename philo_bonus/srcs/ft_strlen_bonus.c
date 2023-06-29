/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:59:38 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/29 16:27:04 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

size_t	ft_strlen(const char *str)
{
	const char	*str_ptr;

	str_ptr = str;
	while (*str_ptr)
		str_ptr++;
	return (str_ptr - str);
}
