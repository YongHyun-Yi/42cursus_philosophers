/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 12:27:31 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/20 12:27:41 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

void	*check_full_eat(void *args)
{
	t_philo_ref	*philo_ref;
	int			cnt;

	philo_ref = (t_philo_ref *)args;
	cnt = philo_ref->number_of_philosophers;
	while (cnt--)
		sem_wait(philo_ref->s_full_eat);
	kill(0, SIGINT);
	return (0);
}

void	*monitoring_is_alive(void *args)
{
	t_philo_stat	*philo_stat;

	philo_stat = (t_philo_stat *)args;
	while (1)
	{
		sem_wait(philo_stat->s_die);
		if (my_gettimeofday() - philo_stat->last_time_to_eat \
		> philo_stat->philo_ref->time_to_die)
		{
			sem_wait(philo_stat->philo_ref->s_is_anyone_die);
			printf("%ld %d %s\n", my_gettimeofday() - philo_stat->philo_ref->\
			start_time, philo_stat->philo_num + 1, "is died\n");
			exit(EXIT_FAILURE);
		}
		sem_post(philo_stat->s_die);
		// usleep(1000 * 1000);
		usleep(200);
	}
}

void	*philo_routine(void *args)
{
	t_philo_stat	*philo_stat;

	philo_stat = (t_philo_stat *)args;
	if (philo_stat->philo_num % 2)
		usleep(100 * philo_stat->philo_ref->number_of_philosophers);
	while (1)
	{
		if (philo_stat->cur_state == THINK)
			philo_think(philo_stat);
		else if (philo_stat->cur_state == EAT)
			philo_eat(philo_stat);
		else
			philo_sleep(philo_stat);
		if (philo_stat->cur_state == EAT || philo_stat->cur_state == SLEEP)
			usleep(get_sleep_time(philo_stat));
	}
	return (0);
}
