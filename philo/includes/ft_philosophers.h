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
		1-1. 먹는 횟수를 다 채웠는지...?
		1-2. 한 명이라도 죽은 철학자가 있는지...?
	2. 동작 수행 및 상태전환 확인
		2-1. 포크를 하나 든 상태에서 대기하는 루프를 어떻게 처리할것인지...?
		2-2. 포크를 든 채로 죽은 철학자는 어떻게 처리할것인지...?
	3. 상태및 조건에 따른 출력

*/

/*

	state 별로 하나의 동작만 한다

	Think 의 경우, 포크1 포크2 상태변환 이렇게 있다면
	첫번째 사이클에서는 포크1을 시도
	두번째 사이클에서는 포크2를 시도
	세번쨰 사이클에서는 상태를 변환

	Eat 의 경우, 포크 내려놓기 상태변환
	첫번째 사이클에 시간체크후 모두 동작?

	Sleep 의 경우, 상태변환
	첫번째 사이클에 시간체크후 상태변환

	사실상 현재코드에서 Think 만 문제가 되는듯
	한 함수안에서 if 로 분기하고 같은 usleep 에 걸리도록 해야하는데
	그것을 분기하려면 포크 쥔 수를 변수로 가지던 해야함
	몇번째 포크를 시도하고있는지 알 수 없기때문
	그렇기에 두 파트로 나눴지만...
	죽었는지 시간도 체크해야하는데...

	모든 상태에서 공통되는 부분
	이미 죽은 스레드가 있는지??
	현재 스레드가 죽었는지??
	이것을 모든 동작마다 체크해야하는가...?
	어차피 출력은 누구하나 죽어있으면 강제로 막아놔서 안찍히지만...

	모든 행동마다 자신이 죽었는지 체크해야한다
	또한 누가 죽었는지도 체크해야한다
	맨 처음에 자신의 상태를 출력한다

	1. 맨 윗부분에서 먹은횟수, 죽었는지 체크를 통해 일괄 종료할수있도록 한다 (플래그로 출력이 막힌 스레드가 여기서 종료된다)
	2. 중간에 시뮬레이션은 진행되어도 출력에서는 죽었는지 체크를 한 후에 출력되니 출력은 막힌다
	3. 시뮬레이션 도중 혹은 대기중 자신이 죽었을때만 실시간으로 출력을 해주고 플래그를 세워야한다
	- 진행은 모두 되지만 출력만 제어한다
	- 매 루프의 첫부분에서 일괄종료하도록 제어한다?

*/

#endif