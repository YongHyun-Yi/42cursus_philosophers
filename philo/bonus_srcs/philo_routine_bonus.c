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

static int	is_all_philo_full(t_philo_ref *philo_ref)
{
	int	ret;

	// pthread_mutex_lock(&philo_ref->m_full_eat);
	ret = (philo_ref->number_of_full_philosophers \
	== philo_ref->number_of_philosophers);
	// pthread_mutex_unlock(&philo_ref->m_full_eat);
	return (ret);
}

static int	get_dead_thread(t_philo_ref *philo_ref)
{
	int	ret;

	// pthread_mutex_lock(&philo_ref->m_die);
	ret = philo_ref->is_anyone_die;
	// pthread_mutex_unlock(&philo_ref->m_die);
	return (ret);
}

static void	set_dead_thread(t_philo_ref *philo_ref, int value)
{
	// pthread_mutex_lock(&philo_ref->m_die);
	philo_ref->is_anyone_die = value;
	// pthread_mutex_unlock(&philo_ref->m_die);
}

static int	is_philo_died(t_philo_stat *philo_stat)
{
	return (my_gettimeofday() - philo_stat->last_time_to_eat \
	> philo_stat->philo_ref->time_to_die);
}

// void	*philo_routine(t_philo_stat	*philo_stat)
void	*philo_routine(void *args)
{
	t_philo_stat	*philo_stat;

	philo_stat = (t_philo_stat *)args;
	if (philo_stat->philo_num % 2)
		usleep(10 * philo_stat->philo_ref->number_of_philosophers);
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
