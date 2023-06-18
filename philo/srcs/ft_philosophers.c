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

void print_philo2(t_philo_stat *philo_stat, char *state)
{
	long print_time;

	pthread_mutex_lock(&philo_stat->philo_ref->m_die);
	if (!philo_stat->philo_ref->is_anyone_die)
	{
		print_time = my_gettimeofday() - philo_stat->philo_ref->start_time;
		printf("%ld %d %s\n", print_time, philo_stat->philo_num, state);
	}
	pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
}

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

int is_all_philo_full(t_philo_ref *philo_ref)
{
	int ret;

	pthread_mutex_lock(&philo_ref->m_full_eat);
	ret = (philo_ref->number_of_full_philosophers == philo_ref->number_of_philosophers);
	pthread_mutex_unlock(&philo_ref->m_full_eat);
	return (ret);
}

int is_philo_died2(t_philo_stat *philo_stat)
{
	return (my_gettimeofday() - philo_stat->last_time_to_eat > philo_stat->philo_ref->time_to_die);
}

int is_philo_died(t_philo_stat *philo_stat, long time)
{
	return (time - philo_stat->last_time_to_eat > philo_stat->philo_ref->time_to_die);
}

int get_dead_thread(t_philo_ref *philo_ref)
{
	int ret;

	// 죽은 스레드 체크
	pthread_mutex_lock(&philo_ref->m_die);
	ret = philo_ref->is_anyone_die;
	pthread_mutex_unlock(&philo_ref->m_die);
	return (ret);
}

void set_dead_thread(t_philo_ref *philo_ref, int value)
{
	pthread_mutex_lock(&philo_ref->m_die);
	philo_ref->is_anyone_die = value;
	pthread_mutex_unlock(&philo_ref->m_die);
}

void down_fork(t_philo_stat *philo_stat)
{
	pthread_mutex_lock(philo_stat->m_fork[0]);
	*philo_stat->fork[0] = 0;
	pthread_mutex_unlock(philo_stat->m_fork[0]);
	pthread_mutex_lock(philo_stat->m_fork[1]);
	*philo_stat->fork[1] = 0;
	pthread_mutex_unlock(philo_stat->m_fork[1]);
}

int take_fork(t_philo_stat *philo_stat, int is_right)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(philo_stat->m_fork[is_right]);
	if (*philo_stat->fork[is_right] == 0)
	{
		*philo_stat->fork[is_right] = 1;
		ret = 1;
	}
	pthread_mutex_unlock(philo_stat->m_fork[is_right]);
	return (ret);
}

void *philo_routine2(void* args) // only mutex
{
	t_philo_stat *philo_stat;
	long cmp_time;

	philo_stat = (t_philo_stat *)args;

	// pthread_mutex_lock(&philo_stat->philo_ref->m_start);
	// pthread_mutex_unlock(&philo_stat->philo_ref->m_start);

	if (philo_stat->philo_num % 2)
	{
		usleep(10 * philo_stat->philo_ref->number_of_philosophers);
		print_philo(philo_stat, philo_stat->last_time_to_eat, "is thinking");
	}

	while (1)
	{
		if (is_all_philo_full(philo_stat->philo_ref))
			return (NULL);
		
		if (get_dead_thread(philo_stat->philo_ref))
		{
			pthread_mutex_unlock(philo_stat->m_fork[0]);
			pthread_mutex_unlock(philo_stat->m_fork[1]);
			// eat 상태에서 이 조건문에 걸리면 unlock 을 하지않고 종료되는게 문제였음...
			// think 는 매번 종료조건을 확인하고 unlock 을 하지만 eat 에서는 다 먹었을때만 unlock 함
			// 어차피 종료조건은 매 루프 초반부에서 확인한다는 전제가 있었기 때문
			// 근데 그 종료조건문에서 unlock 은 빼먹었음 ㅋㅋ;
			return (NULL);
		}
		
		cmp_time = my_gettimeofday();
		
		if (is_philo_died(philo_stat, cmp_time))
		{
			print_philo(philo_stat, cmp_time, "died");
			set_dead_thread(philo_stat->philo_ref, 1);
		}
		
		if (philo_stat->cur_state == THINK)
		{
			// print_philo(philo_stat, my_gettimeofday(), "waiting a fork1");

			pthread_mutex_lock(philo_stat->m_fork[0]);

			print_philo(philo_stat, my_gettimeofday(), "has taken a fork");

			if (is_philo_died(philo_stat, my_gettimeofday()))
			{
				print_philo(philo_stat, my_gettimeofday(), "died");
				set_dead_thread(philo_stat->philo_ref, 1);
				pthread_mutex_unlock(philo_stat->m_fork[0]);
				// printf("down fork1: %d\n", philo_stat->philo_num);
				return (NULL);
			}

			// print_philo(philo_stat, my_gettimeofday(), "waiting a fork2");

			pthread_mutex_lock(philo_stat->m_fork[1]);

			print_philo(philo_stat, my_gettimeofday(), "has taken a fork");

			if (is_philo_died(philo_stat, my_gettimeofday()))
			{
				print_philo(philo_stat, my_gettimeofday(), "died");
				set_dead_thread(philo_stat->philo_ref, 1);
				pthread_mutex_unlock(philo_stat->m_fork[0]);
				// printf("down fork1: %d\n", philo_stat->philo_num);
				pthread_mutex_unlock(philo_stat->m_fork[1]);
				// printf("down fork2: %d\n", philo_stat->philo_num + 1);
				return (NULL);
			}

			philo_stat->cur_state = EAT;
			philo_stat->last_time_to_eat = my_gettimeofday();
			print_philo(philo_stat, philo_stat->last_time_to_eat, "is eating");
		}

		else if (philo_stat->cur_state == EAT)
		{
			if (cmp_time - philo_stat->last_time_to_eat >= philo_stat->philo_ref->time_to_eat)
			{
				pthread_mutex_unlock(philo_stat->m_fork[0]);
				pthread_mutex_unlock(philo_stat->m_fork[1]);

				if (philo_stat->philo_ref->number_of_times_must_eat != -1)
				{
					philo_stat->how_much_eat++;
					if (philo_stat->philo_ref->number_of_times_must_eat == philo_stat->how_much_eat)
					{
						pthread_mutex_lock(&philo_stat->philo_ref->m_full_eat);
						philo_stat->philo_ref->number_of_full_philosophers++;
						pthread_mutex_unlock(&philo_stat->philo_ref->m_full_eat);
					}
				}
				philo_stat->cur_state = SLEEP;
				philo_stat->last_time_to_sleep = cmp_time;
				print_philo(philo_stat, philo_stat->last_time_to_sleep, "is sleeping");
			}
		}

		else
		{
			if (cmp_time - philo_stat->last_time_to_sleep >= philo_stat->philo_ref->time_to_sleep)
			{
				philo_stat->cur_state = THINK;
				print_philo(philo_stat, cmp_time, "is thinking");
				usleep (1000);
			}
		}

		// long sleep_time = 0;
		// if (philo_stat->cur_state == EAT)
		// 	sleep_time = philo_stat->philo_ref->time_to_eat - (cmp_time - philo_stat->last_time_to_eat);
		// else if (philo_stat->cur_state == SLEEP)
		// 	sleep_time = philo_stat->philo_ref->time_to_sleep - (cmp_time - philo_stat->last_time_to_sleep);
		// else if (philo_stat->cur_state == SLEEP)
		// 	sleep_time = philo_stat->philo_ref->time_to_die - (cmp_time - philo_stat->last_time_to_eat);

		// long cmp_time2 = philo_stat->philo_ref->time_to_die - (cmp_time - philo_stat->last_time_to_eat);
		// if (sleep_time > cmp_time2)
		// 	sleep_time = cmp_time2;
		
		// if (sleep_time / 2 > 20)
		// 	usleep (sleep_time / 2);
		// else
		// 	usleep (20);
		usleep(get_sleep_time(philo_stat, cmp_time));
	}
	
	return (NULL);
}

void philo_sleep(t_philo_stat *philo_stat, long cmp_time)
{
	// if (cmp_time - philo_stat->last_time_to_sleep >= philo_stat->philo_ref->time_to_sleep)
	if (my_gettimeofday() - philo_stat->last_time_to_sleep >= philo_stat->philo_ref->time_to_sleep)
	{
		philo_stat->cur_state = THINK;
		// print_philo(philo_stat, cmp_time, "is thinking");
		print_philo2(philo_stat, "is thinking");
		if (philo_stat->philo_num % 2 == 0)
			usleep(200);
	}
}

void philo_eat(t_philo_stat *philo_stat, long cmp_time)
{
	// if (cmp_time - philo_stat->last_time_to_eat >= philo_stat->philo_ref->time_to_eat)
	if (my_gettimeofday() - philo_stat->last_time_to_eat >= philo_stat->philo_ref->time_to_eat)
	{
		down_fork(philo_stat);
		if (philo_stat->philo_ref->number_of_times_must_eat != -1)
		{
			philo_stat->how_much_eat++;
			if (philo_stat->philo_ref->number_of_times_must_eat == philo_stat->how_much_eat)
			{
				pthread_mutex_lock(&philo_stat->philo_ref->m_full_eat);
				philo_stat->philo_ref->number_of_full_philosophers++;
				pthread_mutex_unlock(&philo_stat->philo_ref->m_full_eat);
			}
		}
		philo_stat->cur_state = SLEEP;
		// philo_stat->last_time_to_sleep = cmp_time;
		philo_stat->last_time_to_sleep = my_gettimeofday();
		// print_philo(philo_stat, cmp_time, "is sleeping");
		print_philo2(philo_stat, "is sleeping");
	}
}

void philo_think(t_philo_stat *philo_stat, long cmp_time)
{
	int fork_idx;

	fork_idx = 0;
	while (fork_idx < 2)
	{
		while (!get_dead_thread(philo_stat->philo_ref) && !take_fork(philo_stat, fork_idx))
			usleep (200);
		// cmp_time = my_gettimeofday();
		// if (is_philo_died(philo_stat, cmp_time))
		if (is_philo_died2(philo_stat))
		{
			// print_philo(philo_stat, cmp_time, "died");
			print_philo2(philo_stat, "died");
			set_dead_thread(philo_stat->philo_ref, 1);
			return ;
		}
		// print_philo(philo_stat, cmp_time, "has taken a fork");
		print_philo2(philo_stat, "has taken a fork");
		fork_idx++;
	}
	philo_stat->cur_state = EAT;
	// philo_stat->last_time_to_eat = cmp_time;
	philo_stat->last_time_to_eat = my_gettimeofday();
	// print_philo(philo_stat, philo_stat->last_time_to_eat, "is eating");
	print_philo2(philo_stat, "is eating");
}

long get_sleep_time(t_philo_stat *philo_stat, long cmp_time)
{
	long sleep_time;
	long cmp_time2;

	sleep_time = 0;
	if (philo_stat->cur_state == EAT)
		sleep_time = philo_stat->philo_ref->time_to_eat - (my_gettimeofday() - philo_stat->last_time_to_eat);
	else if (philo_stat->cur_state == SLEEP)
		sleep_time = philo_stat->philo_ref->time_to_sleep - (my_gettimeofday() - philo_stat->last_time_to_sleep);

	cmp_time2 = philo_stat->philo_ref->time_to_die - (my_gettimeofday() - philo_stat->last_time_to_eat);
	if (sleep_time > cmp_time2)
		sleep_time = cmp_time2;
	
	if (sleep_time / 2 > 200)
		return (sleep_time / 2 * 1000);
	else
		return (200);
}

void *philo_routine(void* args) // lock spin
{
	t_philo_stat *philo_stat;
	long cmp_time;

	philo_stat = (t_philo_stat *)args;
	
	if (philo_stat->philo_num % 2)
	{
		usleep(10 * philo_stat->philo_ref->number_of_philosophers);
		// print_philo(philo_stat, my_gettimeofday(), "is thinking");
		print_philo2(philo_stat, "is thinking");
	}
	
	while (1)
	{
		// cmp_time = my_gettimeofday();

		// 모두 다 먹었는지 or 하나라도 종료된 스레드가 있는지 확인
		if (is_all_philo_full(philo_stat->philo_ref)\
		|| get_dead_thread(philo_stat->philo_ref))
			return (NULL);
		
		// 마지막으로 식사한 시간으로부터 생존가능한 시간이 지났는지 확인
		// if (is_philo_died(philo_stat, cmp_time))
		if (is_philo_died2(philo_stat))
		{
			// print_philo(philo_stat, cmp_time, "died");
			print_philo2(philo_stat, "died");
			set_dead_thread(philo_stat->philo_ref, 1);
			return (NULL);
		}

		// 현재 상태에 따라 이벤트처리

		// 생각중인경우 -> 식사 가능여부를 확인
		if (philo_stat->cur_state == THINK)
			// philo_think(philo_stat, cmp_time);
			philo_think(philo_stat, 0);

		// 식사중인 경우 -> 식사 시간초과 확인
		else if (philo_stat->cur_state == EAT)
			philo_eat(philo_stat, cmp_time);

		// 수면중인 경우 -> 수면 시간초과 확인
		else
			philo_sleep(philo_stat, cmp_time);
		
		usleep(get_sleep_time(philo_stat, cmp_time));
	}
	return (0);
}

int philo_thread_create(t_philo_ref *philo_ref, t_philo_stat *philo_arr, int idx)
{
	int lf_idx;
	int rf_idx;
	int rmf_idx;

	lf_idx = (idx % 2 == 1);
	rf_idx = (idx % 2 == 0);
	rmf_idx = (idx + 1) * (idx < philo_ref->number_of_philosophers - 1);
	philo_arr[idx].philo_num = idx;
	philo_arr[idx].fork[lf_idx] = &philo_ref->fork_arr[idx];
	philo_arr[idx].m_fork[lf_idx] = &philo_ref->m_fork_arr[idx];
	philo_arr[idx].fork[rf_idx] = &philo_ref->fork_arr[rmf_idx];
	philo_arr[idx].m_fork[rf_idx] = &philo_ref->m_fork_arr[rmf_idx];
	philo_arr[idx].last_time_to_eat = philo_ref->start_time;
	philo_arr[idx].philo_ref = philo_ref;
	if (pthread_create(&philo_arr[idx].philo_thread, NULL, philo_routine, (void *)&philo_arr[idx]) != 0)
		return (0);
	return (1);
}

int philo_arr_init(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	*philo_arr = (t_philo_stat *)malloc(sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	if (*philo_arr == NULL)
		return (0);
	memset(*philo_arr, 0, sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	return (1);
}

int fork_arr_init(t_philo_ref *philo_ref)
{
	philo_ref->fork_arr = malloc(sizeof(int) * philo_ref->number_of_philosophers);
	if (philo_ref->fork_arr == NULL)
		return (0);
	memset(philo_ref->fork_arr, 0, sizeof(int) * philo_ref->number_of_philosophers);
	return (1);
}

int m_fork_init(t_philo_ref *philo_ref)
{
	int cnt;

	philo_ref->m_fork_arr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * philo_ref->number_of_philosophers);
	if (philo_ref->m_fork_arr == NULL)
		return (0);
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	{
		if (pthread_mutex_init(&philo_ref->m_fork_arr[cnt], NULL) == -1)
			return (0);
		cnt++;
	}
	return (1);
}

int init_philo(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	int cnt;
	
	philo_ref->start_time = my_gettimeofday();
	pthread_mutex_init(&philo_ref->m_die, NULL);
	pthread_mutex_init(&philo_ref->m_full_eat, NULL);
	if (!m_fork_init(philo_ref) || !philo_arr_init(philo_ref, philo_arr)\
	|| !fork_arr_init(philo_ref))
		return (0);
	pthread_mutex_init(&philo_ref->m_start, NULL);//
	pthread_mutex_lock(&philo_ref->m_start);//
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers / 2 + philo_ref->number_of_philosophers % 2)
	{
		if (!philo_thread_create(philo_ref, *philo_arr, cnt++ * 2))
			return (0);
	}
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers / 2)
	{
		if (!philo_thread_create(philo_ref, *philo_arr, cnt++ * 2 + 1))
			return (0);
	}
	pthread_mutex_unlock(&philo_ref->m_start);//
	return (1);
}

int main(int argc, char **argv)
{
	t_philo_ref		philo_ref;
	t_philo_stat	*philo_arr;
	int cnt;

	memset(&philo_ref, 0, sizeof(t_philo_ref));
	

	if (!parse_philo(&philo_ref, argc, argv))
	{
		print_err_msg();
		return (0);
	}
	if (philo_ref.number_of_times_must_eat == 0)
		return (0);
	if (!init_philo(&philo_ref, &philo_arr))
		return (0);
		
	cnt = 0;
	while (cnt < philo_ref.number_of_philosophers)
	{
		pthread_join(philo_arr[cnt].philo_thread, NULL);
		// printf("cnt: %d\n", cnt);
		cnt++;
	}

	return (0);
}