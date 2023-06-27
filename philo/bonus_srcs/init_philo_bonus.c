/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 12:27:18 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/27 11:56:59 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

static void	philo_stat_setup(t_philo_ref *philo_ref, \
t_philo_stat *philo_stat, int idx)
{
	philo_stat->philo_num = idx;
	philo_stat->last_time_to_eat = philo_ref->start_time;
	philo_stat->philo_ref = philo_ref;
	philo_stat->s_die_name = ft_itoa(idx);
	if (!philo_stat->s_die_name)
		kill(0, SIGKILL);
	philo_stat->s_die = sem_open(philo_stat->s_die_name, O_CREAT | O_EXCL, 0, philo_ref->number_of_philosophers);
	if (philo_stat->s_die == SEM_FAILED)
	{
		sem_unlink(philo_stat->s_die_name);
		philo_stat->s_die = sem_open(philo_stat->s_die_name, O_CREAT, 0, philo_ref->number_of_philosophers);
	}

	if (philo_ref->s_full_eat)
		sem_wait(philo_ref->s_full_eat);
	pthread_create(&philo_stat->philo_thread, NULL, philo_routine, philo_stat);
	monitoring_is_alive(philo_stat);
}

void init_sems(t_philo_ref *philo_ref)
{
	philo_ref->s_fork = sem_open("s_fork", O_CREAT | O_EXCL, 0, philo_ref->number_of_philosophers);
	if (philo_ref->s_fork == SEM_FAILED)
	{
		sem_unlink("s_fork");
		philo_ref->s_fork = sem_open("s_fork", O_CREAT, 0, philo_ref->number_of_philosophers);
	}
	if (philo_ref->number_of_times_must_eat != -1)
	{
		philo_ref->s_full_eat = sem_open("s_full_eat", O_CREAT | O_EXCL, 0, philo_ref->number_of_philosophers);
		if (philo_ref->s_full_eat == SEM_FAILED)
		{
			sem_unlink("s_full_eat");
			philo_ref->s_full_eat = sem_open("s_full_eat", O_CREAT, 0, philo_ref->number_of_philosophers);
		}
	}
}

int	init_philo(t_philo_ref *philo_ref, t_philo_stat *philo_stat)
{
	int		cnt;
	pid_t	my_pid;

	philo_ref->start_time = my_gettimeofday();
	
	init_sems(philo_ref);
	
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	{
		my_pid = fork();
		if (my_pid == -1)
			return (0);
		else if (my_pid == 0)
			philo_stat_setup(philo_ref, philo_stat, cnt);
		cnt++;
	}
	// usleep(100 * philo_stat->philo_ref->number_of_philosophers);
	usleep(200);
	if (philo_ref->s_full_eat)
		pthread_create(&philo_ref->full_eet_thread, NULL, check_full_eat, philo_ref);
	return (1);
}
