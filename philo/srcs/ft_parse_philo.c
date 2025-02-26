/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_philo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 12:22:35 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/20 12:27:49 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

void	free_philo(t_philo_ref *philo_ref, t_philo_stat *philo_arr)
{
	int	cnt;

	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	{
		pthread_mutex_destroy(&philo_ref->m_fork_arr[cnt]);
		cnt++;
	}
	pthread_mutex_destroy(philo_ref->m_fork_arr);
	pthread_mutex_destroy(&philo_ref->m_die);
	pthread_mutex_destroy(&philo_ref->m_full_eat);
	free(philo_arr);
	free(philo_ref->m_fork_arr);
	free(philo_ref->fork_arr);
}

int	parse_philo(t_philo_ref *philo_ref, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (0);
	if (!ft_strisnum(argv[1]) || !ft_strisnum(argv[2]) || \
	!ft_strisnum(argv[3]) || !ft_strisnum(argv[4]))
		return (0);
	philo_ref->number_of_philosophers = ft_atoi(argv[1]);
	philo_ref->time_to_die = ft_atoi(argv[2]);
	philo_ref->time_to_eat = ft_atoi(argv[3]);
	philo_ref->time_to_sleep = ft_atoi(argv[4]);
	if (philo_ref->number_of_philosophers <= 0 || philo_ref->time_to_die <= \
	0 || philo_ref->time_to_eat <= 0 || philo_ref->time_to_sleep <= 0)
		return (0);
	philo_ref->number_of_times_must_eat = -1;
	if (argc == 6)
	{
		if (!ft_strisnum(argv[5]) || ft_atoi(argv[5]) < 0)
			return (0);
		philo_ref->number_of_times_must_eat = ft_atoi(argv[5]);
	}
	philo_ref->state_func[THINK] = philo_think;
	philo_ref->state_func[EAT] = philo_eat;
	philo_ref->state_func[SLEEP] = philo_sleep;
	return (1);
}
