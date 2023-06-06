/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:30:01 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/02 12:45:15 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo_info
{
	pthread_t **philo_arr;
	int *fork_arr;
}	t_philo_info;

/*

	⌜ 철학자가 가져야 하는 정보 ⌟

	자신의 번호 -> 인덱스는 0부터이더라도 출력시에 표시되는 철학자의 번호는 1번부터이다
	자신의 현재 상태 (먹는중, 자는중, 생각하는중)
	자신이 마지막으로 먹은 시간, 이 시간으로 상태를 변화시킨다 (이를 기준으로 생사를 판단한다)
	자신이 마지막으로 잠든 시간, 이 시간으로 상태를 변화시킨다

	-
	먹는데 걸리는 시간
	자는데 걸리는 시간
	죽는데 걸리는 시간
	-
	이 셋은 사실상 공통되는 사항이니 상위로 빼기

*/

#endif