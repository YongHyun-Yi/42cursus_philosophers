/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 11:58:06 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/29 16:27:15 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

char	*ft_strdup(const char *s1)
{
	char	*s2;
	char	*s2_ptr;

	s2 = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!s2)
		return (0);
	s2_ptr = s2;
	while (*s1)
		*s2_ptr++ = *s1++;
	*s2_ptr = '\0';
	return (s2);
}
