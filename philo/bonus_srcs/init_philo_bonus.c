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

static int	philo_thread_create(t_philo_ref *philo_ref, \
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
	// if (pthread_create(&philo_arr[idx].philo_thread, NULL, philo_routine, \
	// (void *)&philo_arr[idx]) != 0)
	// 	return (0);
	return (1);
}

static int	fork_arr_init(t_philo_ref *philo_ref)
{
	philo_ref->fork_arr = malloc(sizeof(int) \
	* philo_ref->number_of_philosophers);
	if (philo_ref->fork_arr == NULL)
		return (0);
	memset(philo_ref->fork_arr, 0, sizeof(int) \
	* philo_ref->number_of_philosophers);
	return (1);
}

static int	philo_arr_init(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	*philo_arr = (t_philo_stat *)malloc(sizeof(t_philo_stat) \
	* philo_ref->number_of_philosophers);
	if (*philo_arr == NULL)
		return (0);
	memset(*philo_arr, 0, sizeof(t_philo_stat) \
	* philo_ref->number_of_philosophers);
	return (1);
}

static int	m_fork_init(t_philo_ref *philo_ref)
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
