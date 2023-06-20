/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:29:33 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/20 14:58:09 by yonghyle         ###   ########.fr       */
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

void	print_philo(t_philo_stat *philo_stat, char *state)
{
	long	print_time;

	pthread_mutex_lock(&philo_stat->philo_ref->m_die);
	if (!philo_stat->philo_ref->is_anyone_die)
	{
		print_time = my_gettimeofday() - philo_stat->philo_ref->start_time;
		printf("%ld %d %s\n", print_time, philo_stat->philo_num + 1, state);
	}
	pthread_mutex_unlock(&philo_stat->philo_ref->m_die);
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

static void	print_err_msg(void)
{
	printf("\033[1;31mInvalid arguments!\n");
	printf("\033[1;33mPlease enter the arguments like below ");
	printf("( Numbers must be greater than zero! )\n");
	printf("\033[0;32mNumber of Philosophers, Time to Die, Time to Eat, ");
	printf("Time to Sleep, \033[1;30m[ Number of Times Each Philosopher ");
	printf("Must Eat ]⌟\033[0;0m\n");
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
