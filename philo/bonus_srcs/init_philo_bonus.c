/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 12:27:18 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/20 12:27:41 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

static int	philo_stat(t_philo_ref *philo_ref, \
t_philo_stat *philo_stat, int idx)
{
	philo_stat->philo_num = idx;
	philo_stat->last_time_to_eat = philo_ref->start_time;
	philo_stat->philo_ref = philo_ref;
	fork();
	return (1);
}

int	init_philo(t_philo_ref *philo_ref, t_philo_stat *philo_arr)
{
	int	cnt;
	pid_t my_pid;

	philo_ref->start_time = my_gettimeofday();
	philo_ref->s_fork = sem_open("s_fork", O_CREAT, 0, philo_ref->number_of_philosophers);
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

	
	int cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	{
		my_pid = fork();
		if (my_pid == 0)
			return (1);
		cnt++;
	}
	return (1);
}
