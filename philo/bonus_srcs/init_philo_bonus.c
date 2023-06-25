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

static void	philo_stat_setup(t_philo_ref *philo_ref, \
t_philo_stat *philo_stat, int idx)
{
	philo_stat->philo_num = idx;
	philo_stat->last_time_to_eat = philo_ref->start_time;
	philo_stat->philo_ref = philo_ref;
	philo_routine(philo_stat);
}

int	init_philo(t_philo_ref *philo_ref, t_philo_stat *philo_stat)
{
	int		cnt;
	pid_t	my_pid;

	philo_ref->start_time = my_gettimeofday();
	philo_ref->s_fork = sem_open("s_fork", O_CREAT, 0, philo_ref->number_of_philosophers);
	// return (0);
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	{
		my_pid = fork();
		if (my_pid == -1)
			return (0);
		else if (my_pid == 0)
			philo_stat_setup(philo_ref, philo_stat, cnt * 2);
		cnt++;
	}
	return (1);
}
