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
# include <limits.h>

typedef struct s_philo_ref
{
	int number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_must_eat;
	int is_anyone_die;
}	t_philo_ref;

typedef struct s_philo_info
{
	pthread_t *philo_arr;
	int *fork_arr;
}	t_philo_info;

typedef struct s_philo_stat
{
	int lifetime;
}	t_philo_stat;

int	ft_strisnum(char *str);
int	ft_atoi(const char *str);

/*

	⌜ 철학자가 가져야 하는 정보 ⌟

	자신의 번호 -> 인덱스는 0부터이더라도 출력시에 표시되는 철학자의 번호는 1번부터이다
	자신의 현재 상태 (먹는중, 자는중, 생각하는중)
	자신이 마지막으로 먹은 시간, 이 시간으로 상태를 변화시킨다 (이를 기준으로 생사를 판단한다)
	자신이 마지막으로 잠든 시간, 이 시간으로 상태를 변화시킨다
	자신이 몇 번 먹었는지

	-
	먹는데 걸리는 시간
	자는데 걸리는 시간
	죽는데 걸리는 시간
	먹어야 하는 횟수
	-
	이 셋은 사실상 공통되는 사항이니 상위로 빼기
	모든 철학자가 같이 체크해봐야 하는거
	포크, 누구하나라도 죽었는지, 각자 먹은횟수(마지막 인자가 들어왔을경우)
	각자 먹은 횟수는 개인으로 뺴는게 나은가?

	매번 상태를 확인할때마다 출력하는게 아니라
	상태를 변경시킬때만 새로운 상태를 체크하도록
	성능문제를 개선하기위해 빠른속도로 자신을 검사하는게 아니라 usleep으로
	일정시간 텀을 두고 검사하도록 (대신 기다리다 죽지 않도록 잘 조절하기)
	자는 도중에 죽으면...? (자는 동안 통짜로 sleep이 아니라 그냥 상태만 변경하고 계속적으로 상태검사를 하는건...?)

*/

#endif