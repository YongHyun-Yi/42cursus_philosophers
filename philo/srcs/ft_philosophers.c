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

void	print_philo(t_philo_stat *philo_stat, char *state)
{
	long	print_time;

	pthread_mutex_lock(&philo_stat->philo_ref->m_die);
	if (!philo_stat->philo_ref->is_anyone_die)
	{
		print_time = my_gettimeofday() - philo_stat->philo_ref->start_time;
		printf("%ld %d %s\n", print_time, philo_stat->philo_num, state);
	}
	pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
}

void	print_err_msg(void)
{
	printf("\033[1;31mInvalid arguments!\n");
	printf("\033[1;33mPlease enter the arguments like below ");
	printf("( Numbers must be greater than zero! )\n");
	printf("\033[0;32mNumber of Philosophers, Time to Die, Time to Eat, ");
	printf("Time to Sleep, \033[1;30m[ Number of Times Each Philosopher ");
	printf("Must Eat ]⌟\033[0;0m\n");
}

int	is_all_philo_full(t_philo_ref *philo_ref)
{
	int	ret;

	pthread_mutex_lock(&philo_ref->m_full_eat);
	ret = (philo_ref->number_of_full_philosophers \
	== philo_ref->number_of_philosophers);
	pthread_mutex_unlock(&philo_ref->m_full_eat);
	return (ret);
}

int	is_philo_died(t_philo_stat *philo_stat)
{
	return (my_gettimeofday() - philo_stat->last_time_to_eat \
	> philo_stat->philo_ref->time_to_die);
}

int	get_dead_thread(t_philo_ref *philo_ref)
{
	int	ret;

	pthread_mutex_lock(&philo_ref->m_die);
	ret = philo_ref->is_anyone_die;
	pthread_mutex_unlock(&philo_ref->m_die);
	return (ret);
}

void	set_dead_thread(t_philo_ref *philo_ref, int value)
{
	pthread_mutex_lock(&philo_ref->m_die);
	philo_ref->is_anyone_die = value;
	pthread_mutex_unlock(&philo_ref->m_die);
}

void	down_fork(t_philo_stat *philo_stat)
{
	pthread_mutex_lock(philo_stat->m_fork[0]);
	*philo_stat->fork[0] = 0;
	pthread_mutex_unlock(philo_stat->m_fork[0]);
	pthread_mutex_lock(philo_stat->m_fork[1]);
	*philo_stat->fork[1] = 0;
	pthread_mutex_unlock(philo_stat->m_fork[1]);
	philo_stat->fork_idx = 0;
}

int	take_fork(t_philo_stat *philo_stat, int is_right)
{
	int	ret;

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

void	philo_sleep(t_philo_stat *philo_stat)
{
	if (my_gettimeofday() - philo_stat->last_time_to_sleep \
	>= philo_stat->philo_ref->time_to_sleep)
	{
		philo_stat->cur_state = THINK;
		print_philo(philo_stat, "is thinking");
		if (philo_stat->philo_ref->number_of_philosophers % 2)
			usleep ((philo_stat->philo_ref->time_to_eat \
			- philo_stat->philo_ref->time_to_sleep) * 1000);
	}
}

void	philo_eat(t_philo_stat *philo_stat)
{
	if (my_gettimeofday() - philo_stat->last_time_to_eat \
	>= philo_stat->philo_ref->time_to_eat)
	{
		down_fork(philo_stat);
		if (philo_stat->philo_ref->number_of_times_must_eat != -1)
		{
			philo_stat->how_much_eat++;
			if (philo_stat->philo_ref->number_of_times_must_eat \
			== philo_stat->how_much_eat)
			{
				pthread_mutex_lock(&philo_stat->philo_ref->m_full_eat);
				philo_stat->philo_ref->number_of_full_philosophers++;
				pthread_mutex_unlock(&philo_stat->philo_ref->m_full_eat);
			}
		}
		philo_stat->cur_state = SLEEP;
		philo_stat->last_time_to_sleep = my_gettimeofday();
		print_philo(philo_stat, "is sleeping");
	}
}

void	philo_think(t_philo_stat *philo_stat)
{
	while (philo_stat->fork_idx < 2)
	{
		if (!take_fork(philo_stat, philo_stat->fork_idx))
			return ;
		print_philo(philo_stat, "has taken a fork");
		philo_stat->fork_idx++;
	}
	philo_stat->cur_state = EAT;
	philo_stat->last_time_to_eat = my_gettimeofday();
	print_philo(philo_stat, "is eating");
}

long	get_sleep_time(t_philo_stat *philo_stat)
{
	long	sleep_time;

	sleep_time = 0;
	if (philo_stat->cur_state == THINK)
		return (200);
	if (philo_stat->cur_state == EAT)
		sleep_time = philo_stat->philo_ref->time_to_eat \
		- (my_gettimeofday() - philo_stat->last_time_to_eat);
	else if (philo_stat->cur_state == SLEEP)
		sleep_time = philo_stat->philo_ref->time_to_sleep \
		- (my_gettimeofday() - philo_stat->last_time_to_sleep);
	if (sleep_time / 2 > 20)
		return (sleep_time / 2 * 100);
	else
		return (20 * 100);
}

void	*philo_routine(void *args)
{
	t_philo_stat	*philo_stat;

	philo_stat = (t_philo_stat *)args;
	if (philo_stat->philo_num % 2)
		usleep(10 * philo_stat->philo_ref->number_of_philosophers);
	while (1)
	{
		if (is_all_philo_full(philo_stat->philo_ref) \
		|| get_dead_thread(philo_stat->philo_ref))
			return (NULL);
		if (is_philo_died(philo_stat))
		{
			print_philo(philo_stat, "died");
			set_dead_thread(philo_stat->philo_ref, 1);
			return (NULL);
		}
		philo_stat->philo_ref->state_func[philo_stat->cur_state](philo_stat);
		usleep(get_sleep_time(philo_stat));
	}
	return (0);
}

int	philo_thread_create(t_philo_ref *philo_ref, \
t_philo_stat *philo_arr, int idx)
{
	int	lf_idx;
	int	rf_idx;
	int	rmf_idx;

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
	if (pthread_create(&philo_arr[idx].philo_thread, NULL, philo_routine, \
	(void *)&philo_arr[idx]) != 0)
		return (0);
	return (1);
}

int	philo_arr_init(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	*philo_arr = (t_philo_stat *)malloc(sizeof(t_philo_stat) \
	* philo_ref->number_of_philosophers);
	if (*philo_arr == NULL)
		return (0);
	memset(*philo_arr, 0, sizeof(t_philo_stat) \
	* philo_ref->number_of_philosophers);
	return (1);
}

int	fork_arr_init(t_philo_ref *philo_ref)
{
	philo_ref->fork_arr = malloc(sizeof(int) \
	* philo_ref->number_of_philosophers);
	if (philo_ref->fork_arr == NULL)
		return (0);
	memset(philo_ref->fork_arr, 0, sizeof(int) \
	* philo_ref->number_of_philosophers);
	return (1);
}

int	m_fork_init(t_philo_ref *philo_ref)
{
	int	cnt;

	philo_ref->m_fork_arr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) \
	* philo_ref->number_of_philosophers);
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

int	init_philo(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	int	cnt;

	philo_ref->start_time = my_gettimeofday();
	pthread_mutex_init(&philo_ref->m_die, NULL);
	pthread_mutex_init(&philo_ref->m_full_eat, NULL);
	if (!m_fork_init(philo_ref) || !philo_arr_init(philo_ref, philo_arr) \
	|| !fork_arr_init(philo_ref))
		return (0);
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers / 2 \
	+ philo_ref->number_of_philosophers % 2)
	{
		if (!philo_thread_create(philo_ref, *philo_arr, cnt * 2))
			return (0);
		cnt++;
	}
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers / 2)
	{
		if (!philo_thread_create(philo_ref, *philo_arr, cnt * 2 + 1))
			return (0);
		cnt++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo_ref		philo_ref;
	t_philo_stat	*philo_arr;
	int				cnt;

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
		cnt++;
	}
	return (0);
}
