/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 12:27:35 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/29 16:23:15 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers_bonus.h"

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
		sem_post(philo_stat->philo_ref->s_fork);
		sem_post(philo_stat->philo_ref->s_fork);
		if (philo_stat->philo_ref->number_of_times_must_eat != -1)
		{
			philo_stat->how_much_eat++;
			if (philo_stat->philo_ref->number_of_times_must_eat \
			== philo_stat->how_much_eat)
				sem_post(philo_stat->philo_ref->s_full_eat);
		}
		philo_stat->cur_state = SLEEP;
		philo_stat->last_time_to_sleep = my_gettimeofday();
		print_philo(philo_stat, "is sleeping");
	}
}

void	philo_think(t_philo_stat *philo_stat)
{
	sem_post(philo_stat->s_die);
	sem_wait(philo_stat->philo_ref->s_fork);
	print_philo(philo_stat, "has taken a fork");
	sem_wait(philo_stat->philo_ref->s_fork);
	print_philo(philo_stat, "has taken a fork");
	sem_wait(philo_stat->s_die);
	print_philo(philo_stat, "is eating");
	philo_stat->cur_state = EAT;
	philo_stat->last_time_to_eat = my_gettimeofday();
}
