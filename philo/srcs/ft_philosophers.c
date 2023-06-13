/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:29:33 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/02 12:29:35 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

void print_philo(t_philo_stat *philo_stat, long time, char *state)
{
	long print_time;

	pthread_mutex_lock(&philo_stat->philo_ref->m_die);
	if (!philo_stat->philo_ref->is_anyone_die)
	{
		print_time = time - philo_stat->philo_ref->start_time;
		printf("%ld %d %s\n", print_time, philo_stat->philo_num, state);
	}
	pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
}

void print_err_msg(void)
{
	printf("\033[1;31mInvalid arguments!\n");
	printf("\033[1;33mPlease enter the arguments like below ");
	printf("( Numbers must be greater than zero! )\n");
	printf("\033[0;32mNumber of Philosophers, Time to Die, Time to Eat, ");
	printf("Time to Sleep, \033[1;30m[ Number of Times Each Philosopher ");
	printf("Must Eat ]⌟\033[0;0m\n");
}

int take_fork(t_philo_stat *philo_stat, int is_right)
{
	int ret;
	pthread_mutex_t *target_m_fork;
	int *target_fork;

	target_m_fork = philo_stat->m_fork[0];
	target_fork = philo_stat->fork[0];
	if (is_right)
	{
		target_m_fork = philo_stat->m_fork[1];
		target_fork = philo_stat->fork[1];
	}
	ret = 0;
	pthread_mutex_lock(target_m_fork);
	if (*target_fork == 0)
	{
		*target_fork = 1;
		ret = 1;
	}
	pthread_mutex_unlock(target_m_fork);
	return (ret);
}

void *philo_routine(void* args)
{
	t_philo_stat *philo_stat;

	philo_stat = (t_philo_stat *)args;

	// if (philo_stat->philo_num % 2)
	// 	usleep(200);
	
	while (1)
	{
		// 먹어야 하는 횟수 0 일경우 바로 종료하도록 확인
		if (philo_stat->philo_ref->number_of_times_must_eat == philo_stat->how_much_eat)
			return (NULL);

		// 아래의 두 케이스에서 죽은 철학자가 포크를 쥐고있었다면 포크를 놔준다?
		// 그래도 시뮬레이션은 계속되어야 한다면 (출력은 없이)
		// 일괄적으로 끝낼수 있는 방법은...?

		// 하나라도 종료된 스레드가 있는지 확인
		pthread_mutex_lock(&philo_stat->philo_ref->m_die);
		if (philo_stat->philo_ref->is_anyone_die)
		{
			pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
			return (NULL);
		}
		pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
		
		long cmp_time = my_gettimeofday();

		// 마지막으로 식사한 시간으로부터 생존가능한 시간이 지났는지 확인
		if (cmp_time - philo_stat->last_time_to_eat > philo_stat->philo_ref->time_to_die)
		{
			print_philo(philo_stat, my_gettimeofday(), "died");
			pthread_mutex_lock(&philo_stat->philo_ref->m_die);
			philo_stat->philo_ref->is_anyone_die = 1;
			pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
			return (NULL);
		}

		// 현재 상태에 따라 이벤트처리
		// 포크는 다른 스레드도 접근할수있는 ref 구조체에 있기 때문에 포크별로 mutex를 생성했음

		// 생각중인경우 -> 식사 가능여부를 확인
		if (philo_stat->cur_state == THINK)
		{
			// 함수화 버전
			if (take_fork(philo_stat, 0))
			{
				print_philo(philo_stat, my_gettimeofday(), "has taken a fork");//1

				int idx;
				if (philo_stat->philo_num == philo_stat->philo_ref->number_of_philosophers - 1)
					idx = 0;
				else
					idx = philo_stat->philo_ref->number_of_philosophers + 1;
				
				while (1)
				{
					if (take_fork(philo_stat, 1))// 1 0
					{
						print_philo(philo_stat, my_gettimeofday(), "has taken a fork");//2

						philo_stat->cur_state = EAT;
						philo_stat->last_time_to_eat = my_gettimeofday();
						print_philo(philo_stat, philo_stat->last_time_to_eat, "is eating");
						break;
					}
					else
					{
						pthread_mutex_lock(&philo_stat->philo_ref->m_die);
						if (philo_stat->philo_ref->is_anyone_die)
						{
							pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
							break;
						}
						pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
						usleep(20);
					}
				}
				// printf("THINK WHILE BREAK\n");
			}
			// else
			// {
			// 	print_philo(philo_stat, my_gettimeofday(), "cant take fork");
			// }
		}

		// 식사중인 경우 -> 식사 시간초과 확인
		else if (philo_stat->cur_state == EAT)
		{
			cmp_time = my_gettimeofday();

			// 식사 시작으로부터 time_to_eat 만큼의 시간이 경과했을때
			if (cmp_time - philo_stat->last_time_to_eat >= philo_stat->philo_ref->time_to_eat)
			{
				// 첫번째 포크의 상태를 변경
				pthread_mutex_lock(philo_stat->m_fork[0]);
				*philo_stat->fork[0] = 0;
				pthread_mutex_unlock(philo_stat->m_fork[0]);

				// 두번째 포크의 상태를 변경
				pthread_mutex_lock(philo_stat->m_fork[1]);
				*philo_stat->fork[1] = 0;
				pthread_mutex_unlock(philo_stat->m_fork[1]);

				// 먹어야 하는 횟수가 정해져있는 경우 +1 계산
				// 모두 채웠으면 종료
				if (philo_stat->philo_ref->number_of_times_must_eat != -1)
				{
					philo_stat->how_much_eat += 1;
					if (philo_stat->philo_ref->number_of_times_must_eat == philo_stat->how_much_eat)
					{
						pthread_mutex_lock(&philo_stat->philo_ref->m_full_eat);
						philo_stat->philo_ref->number_of_full_philosophers++;
						pthread_mutex_unlock(&philo_stat->philo_ref->m_full_eat);
					}
				}
				
				// 철학자의 상태를 변경
				philo_stat->cur_state = SLEEP;

				// 마지막으로 수면한 시간을 갱신
				philo_stat->last_time_to_sleep = my_gettimeofday();

				print_philo(philo_stat, philo_stat->last_time_to_sleep, "is sleeping");
			}
		}

		// 수면중인 경우 -> 수면 시간초과 확인
		else
		{
			cmp_time = my_gettimeofday();

			// 마지막으로 수면한 시간으로부터 time_to_sleep 만큼의 시간이 경과했을때
			if (cmp_time - philo_stat->last_time_to_sleep >= philo_stat->philo_ref->time_to_sleep)
			{
				// 철학자의 상태를 변경
				philo_stat->cur_state = THINK;

				print_philo(philo_stat, my_gettimeofday(), "is thinking");
			}
		}

		// usleep(20);
		long sleep_time = 0;
		if (philo_stat->cur_state == EAT)
			sleep_time = philo_stat->philo_ref->time_to_eat - (my_gettimeofday() - philo_stat->last_time_to_eat);
		else if (philo_stat->cur_state == SLEEP)
			sleep_time = philo_stat->philo_ref->time_to_sleep - (my_gettimeofday() - philo_stat->last_time_to_sleep);
		else if (philo_stat->cur_state == THINK)
		{
			// printf("THINK wait\n");
			// sleep_time = 20;
			sleep_time = philo_stat->philo_ref->time_to_die - (my_gettimeofday() - philo_stat->last_time_to_eat);
		}

		if (sleep_time > philo_stat->philo_ref->time_to_die - (my_gettimeofday() - philo_stat->last_time_to_eat))
			sleep_time = philo_stat->philo_ref->time_to_die - (my_gettimeofday() - philo_stat->last_time_to_eat);
		
		if (sleep_time / 2 > 20)
			usleep (sleep_time / 2);
		else
			usleep (20);

		// long sleep_time = 0;
		// if (philo_stat->cur_state == EAT)
		// 	sleep_time = (my_gettimeofday() - philo_stat->last_time_to_eat);
		// else if (philo_stat->cur_state == SLEEP)
		// 	sleep_time = (my_gettimeofday() - philo_stat->last_time_to_sleep);
		// if (sleep_time > 200)
		// usleep (sleep_time / 2 * 1000);
	}
}

int init_philo(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	int cnt;
	
	philo_ref->start_time = my_gettimeofday();
	pthread_mutex_init(&philo_ref->m_die, NULL);
	pthread_mutex_init(&philo_ref->m_full_eat, NULL);

	philo_ref->m_fork_arr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * philo_ref->number_of_philosophers);
	if (philo_ref->m_fork_arr == NULL)
		return (0);
	
	for (int i = 0; i < philo_ref->number_of_philosophers; i++)
		pthread_mutex_init(&philo_ref->m_fork_arr[i], NULL);
	
	*philo_arr = (t_philo_stat *)malloc(sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	if (*philo_arr == NULL)
		return (0);
	memset(*philo_arr, 0, sizeof(t_philo_stat) * philo_ref->number_of_philosophers);

	philo_ref->fork_arr = malloc(sizeof(int) * philo_ref->number_of_philosophers);
	if (philo_ref->fork_arr == NULL)
		return (0);
	memset(philo_ref->fork_arr, 0, sizeof(int) * philo_ref->number_of_philosophers);
	
	// cnt = 0;
	// while (cnt < philo_ref->number_of_philosophers)
	// {
	// 	(*philo_arr)[cnt].philo_num = cnt;
	// 	(*philo_arr)[cnt].fork[0] = &philo_ref->fork_arr[cnt];
	// 	(*philo_arr)[cnt].m_fork[0] = &philo_ref->m_fork_arr[cnt];
	// 	if (cnt - 1 == philo_ref->number_of_philosophers)
	// 	{
	// 		(*philo_arr)[cnt].fork[1] = &philo_ref->fork_arr[0];
	// 		(*philo_arr)[cnt].m_fork[1] = &philo_ref->m_fork_arr[0];
	// 	}
	// 	else
	// 	{
	// 		(*philo_arr)[cnt].fork[1] = &philo_ref->fork_arr[cnt + 1];
	// 		(*philo_arr)[cnt].m_fork[1] = &philo_ref->m_fork_arr[cnt + 1];
	// 	}
	// 	(*philo_arr)[cnt].last_time_to_eat = philo_ref->start_time;
	// 	(*philo_arr)[cnt].philo_ref = philo_ref;
	// 	pthread_create(&(*philo_arr)[cnt].philo_thread, NULL, philo_routine, (void *)&(*philo_arr)[cnt]);
	// 	cnt++;
	// }

	// 짝수 철학자부터 일괄 생성
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers / 2)
	{
		(*philo_arr)[cnt].philo_num = cnt * 2;
		(*philo_arr)[cnt].fork[0] = &philo_ref->fork_arr[cnt];
		(*philo_arr)[cnt].m_fork[0] = &philo_ref->m_fork_arr[cnt];
		if (cnt - 1 == philo_ref->number_of_philosophers)
		{
			(*philo_arr)[cnt].fork[1] = &philo_ref->fork_arr[0];
			(*philo_arr)[cnt].m_fork[1] = &philo_ref->m_fork_arr[0];
		}
		else
		{
			(*philo_arr)[cnt].fork[1] = &philo_ref->fork_arr[cnt + 1];
			(*philo_arr)[cnt].m_fork[1] = &philo_ref->m_fork_arr[cnt + 1];
		}
		(*philo_arr)[cnt].last_time_to_eat = philo_ref->start_time;
		(*philo_arr)[cnt].philo_ref = philo_ref;
		pthread_create(&(*philo_arr)[cnt].philo_thread, NULL, philo_routine, (void *)&(*philo_arr)[cnt]);
		cnt++;
	}

	// 홀수 철학자 일괄 생성
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers / 2 + philo_ref->number_of_philosophers % 2)
	{
		(*philo_arr)[cnt].philo_num = cnt * 2 + 1;
		(*philo_arr)[cnt].fork[0] = &philo_ref->fork_arr[cnt];
		(*philo_arr)[cnt].m_fork[0] = &philo_ref->m_fork_arr[cnt];
		if (cnt - 1 == philo_ref->number_of_philosophers)
		{
			(*philo_arr)[cnt].fork[1] = &philo_ref->fork_arr[0];
			(*philo_arr)[cnt].m_fork[1] = &philo_ref->m_fork_arr[0];
		}
		else
		{
			(*philo_arr)[cnt].fork[1] = &philo_ref->fork_arr[cnt + 1];
			(*philo_arr)[cnt].m_fork[1] = &philo_ref->m_fork_arr[cnt + 1];
		}
		(*philo_arr)[cnt].last_time_to_eat = philo_ref->start_time;
		(*philo_arr)[cnt].philo_ref = philo_ref;
		pthread_create(&(*philo_arr)[cnt].philo_thread, NULL, philo_routine, (void *)&(*philo_arr)[cnt]);
		cnt++;
	}
	return (1);
}

int parse_philo(t_philo_ref *philo_ref, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (0);
	if (!ft_strisnum(argv[1]) || !ft_strisnum(argv[2])\
	|| !ft_strisnum(argv[3]) || !ft_strisnum(argv[4]))
		return (0);
	philo_ref->number_of_philosophers = ft_atoi(argv[1]);
	philo_ref->time_to_die = ft_atoi(argv[2]);
	philo_ref->time_to_eat = ft_atoi(argv[3]);
	philo_ref->time_to_sleep = ft_atoi(argv[4]);
	if (philo_ref->number_of_philosophers <= 0 || philo_ref->time_to_die <= 0\
	|| philo_ref->time_to_eat <= 0 || philo_ref->time_to_sleep <= 0)
		return (0);
	philo_ref->number_of_times_must_eat = -1;
	if (argc == 6)
	{
		if (!ft_strisnum(argv[5]) || argv[5] <= 0)
			return (0);
		philo_ref->number_of_times_must_eat = ft_atoi(argv[5]);
	}
	return (1);
}

int main(int argc, char **argv)
{
	t_philo_ref		philo_ref;
	t_philo_stat	*philo_arr;
	int cnt;

	(void)argc;
	(void)argv;
	memset(&philo_ref, 0, sizeof(t_philo_ref));

	if (!parse_philo(&philo_ref, argc, argv))
	{
		print_err_msg();
		return (0);
	}

	if (!init_philo(&philo_ref, &philo_arr))
		return (0);

	cnt = 0;
	while (cnt < philo_ref.number_of_philosophers)
	{
		pthread_join(philo_arr[cnt].philo_thread, NULL);
		cnt++;
	}

	return (0);
}