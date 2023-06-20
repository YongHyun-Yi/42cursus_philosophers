/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 12:27:35 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/20 12:27:40 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

static void	down_fork(t_philo_stat *philo_stat)
{
	pthread_mutex_lock(philo_stat->m_fork[0]);
	*philo_stat->fork[0] = 0;
	pthread_mutex_unlock(philo_stat->m_fork[0]);
	pthread_mutex_lock(philo_stat->m_fork[1]);
	*philo_stat->fork[1] = 0;
	pthread_mutex_unlock(philo_stat->m_fork[1]);
	philo_stat->fork_idx = 0;
}

static int	take_fork(t_philo_stat *philo_stat, int is_right)
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
