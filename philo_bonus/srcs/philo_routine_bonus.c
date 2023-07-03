/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 12:27:31 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/29 16:23:36 by yonghyle         ###   ########.fr       */
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
	printf("eat done\n");
	kill(0, SIGINT);
	return (0);
}

int	is_philo_died(const t_philo_stat *philo_stat)
{
	int	ret;

	printf("num: %d in philo died\n", philo_stat->philo_num + 1);
	ret = my_gettimeofday() - philo_stat->last_time_to_eat \
	> philo_stat->philo_ref->time_to_die;
	return (ret);
}

void	set_philo_died(t_philo_stat	*philo_stat)
{
	sem_wait(philo_stat->philo_ref->s_is_anyone_die);
	printf("%ld %d %s\n", my_gettimeofday() - philo_stat->philo_ref->\
	start_time, philo_stat->philo_num + 1, "is died\n");
	exit(EXIT_FAILURE);
}

void	*monitoring_is_alive(void *args)
{
	t_philo_stat	*philo_stat;

	philo_stat = (t_philo_stat *)args;
	while (1)
	{
		sem_wait(philo_stat->s_die);
		if (is_philo_died(philo_stat))
			set_philo_died(philo_stat);
		sem_post(philo_stat->s_die);
		usleep(5000);
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
		if (is_philo_died(philo_stat))
			set_philo_died(philo_stat);
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
