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
	int number_of_full_philosophers;
	int is_anyone_die;
	pthread_mutex_t m_die;
	pthread_mutex_t m_full_eat;
	pthread_mutex_t *m_fork_arr;
	int *fork_arr;
	long start_time;
}	t_philo_ref;

typedef struct s_philo_stat
{
	t_philo_ref *philo_ref;
	pthread_t philo_thread;
	int philo_num;
	int *fork[2];
	pthread_mutex_t *m_fork[2];
	int cur_state;
	int how_much_eat;
	long last_time_to_eat;
	long last_time_to_sleep;
}	t_philo_stat;

typedef enum e_philo_state
{
	THINK,
	EAT,
	SLEEP
}	t_philo_state;

int	ft_strisnum(char *str);
int	ft_atoi(const char *str);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);
long my_gettimeofday(void);

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

/*

	스레드 생성
	check 라는 mutex에 lock을 시도
	열려있다면 lock을 한 후
	1. is_anyone_die 를 참조하여 누군가 죽었는지 확인, 죽었으면 스레드 종료
	2. last_time_to_eat 과 time_to_die 를 비교하여 죽었는지 확인, 죽었으면 is_anyone_die 를 업데이트하고 스레드 종료
	3. cur_state 를 참조하여 think 라면 fork 배열애서 내가 사용해야하는 fork 들의 상태를 확인
	3-1. fork 가 사용가능하다면 fork 의 상태를 변경하고 state 를 변경 후 last_time_to_eat 을 수행
	3-2. fork 가 사용불가능하다면 그대로 pass
	4. eat 라면 last_time_to_eat 과 time_to_eat 을 비교하여 다 먹었는지 확인
	4-1. 다 먹었다면 fork 의 상태를 변경하고 state 를 변경
	4-1-1. 먹은 횟수를 세야한다면 how_much_eat +1 후 number_of_times_must_eat 과 how_much_eat 를 비교, 충족하면 스레드 종료
	4-2. 아직 더 먹어야 한다면 그대로 pass
	5. sleep 이라면 last_time_to_sleep 과 time_to_sleep 을 비교
	5-1. 다 잤다면 state 를 변경
	5-2. 아직 더 자야 한다면 그대로 pass
	6. 위의 과정이 모두 끝나면 check 를 mutex_unlock 을 수행

	@ 필요하다면 같은 스레드가 곧바로 cpu를 다시 할당받는것을 방지하기위해 usleep 을 걸어주기...?

*/

/*

	1. 종료조건 확인
	2. 동작 수행 및 상태전환 확인
	3. 상태및 조건에 따른 출력

*/

#endif