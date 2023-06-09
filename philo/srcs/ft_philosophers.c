/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:29:33 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/02 12:29:35 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_philosophers.h"

void print_err_msg(void)
{
	printf("\033[1;31mInvalid arguments!\n");
	printf("\033[1;33mPlease enter the arguments like below ");
	printf("( Numbers must be greater than zero! )\n");
	printf("\033[0;32mNumber of Philosophers, Time to Die, Time to Eat, ");
	printf("Time to Sleep, \033[1;30m[ Number of Times Each Philosopher ");
	printf("Must Eat ]⌟\033[0;0m\n");
}

void *philo_routine(void* args)
{
	t_philo_stat *philo_stat;

	philo_stat = (t_philo_stat *)args;
	// if (philo_stat->philo_num % 2)
		// usleep(200);
	// pthread_mutex_lock(&philo_stat->philo_ref->check);
	// write(1, "I'm philosopher ", 16);
	// ft_putnbr_fd(philo_stat->philo_num, 1);
	// write(1, "\n", 1);
	printf("I'm philosopher %d\n", philo_stat->philo_num);
	// pthread_mutex_unlock(&philo_stat->philo_ref->check);
	
	return (NULL);
}

int init_philo(t_philo_ref *philo_ref, t_philo_stat *philo_arr)
{
	int cnt;

	pthread_mutex_init(&philo_ref->check, NULL);
	philo_arr = (t_philo_stat *)malloc(sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	if (philo_arr == NULL)
		return (0);
	memset(philo_arr, -1, sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	{
		philo_arr[cnt].philo_num = cnt;
		philo_arr[cnt].philo_ref = philo_ref;
		pthread_create(&philo_arr[cnt].philo_thread, NULL, philo_routine, (void *)&philo_arr[cnt]);
		cnt++;
	}
	return (1);
}

int parse_philo(t_philo_ref *philo_ref, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (0);
	if (!ft_strisnum(argv[1]) || !ft_strisnum(argv[2])\
	|| !ft_strisnum(argv[3]) || !ft_strisnum(argv[4]))
		return (0);
	philo_ref->number_of_philosophers = ft_atoi(argv[1]);
	philo_ref->time_to_die = ft_atoi(argv[2]);
	philo_ref->time_to_eat = ft_atoi(argv[3]);
	philo_ref->time_to_sleep = ft_atoi(argv[4]);
	if (philo_ref->number_of_philosophers <= 0 || philo_ref->time_to_die <= 0\
	|| philo_ref->time_to_eat <= 0 || philo_ref->time_to_sleep <= 0)
		return (0);
	philo_ref->number_of_times_must_eat = -1;
	if (argc == 6)
	{
		if (!ft_strisnum(argv[5]) || argv[5] <= 0)
			return (0);
		philo_ref->number_of_times_must_eat = ft_atoi(argv[5]);
	}
	return (1);
}

int main(int argc, char **argv)
{
	t_philo_ref		philo_ref;
	t_philo_stat	*philo_arr;
	int cnt;

	(void)argc;
	(void)argv;
	memset(&philo_ref, 0, sizeof(t_philo_ref));

	if (!parse_philo(&philo_ref, argc, argv))
	{
		print_err_msg();
		return (0);
	}

	if (!init_philo(&philo_ref, philo_arr))
		return (0);
	// sleep(1);
	cnt = 0;
	while (cnt < philo_ref.number_of_philosophers)
	{
		// pthread_join(*philo_arr[cnt].philo_thread, NULL);
		pthread_join(philo_arr[cnt].philo_thread, NULL);
		cnt++;
	}
	// pthread_join(philo_arr[5].philo_thread, NULL);
	// for (int i = 0; i < philo_ref.number_of_philosophers; i++)
		// pthread_join(philo_arr[i].philo_thread, NULL);
	
	return (0);
}