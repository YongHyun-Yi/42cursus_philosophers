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

void *foo(void *arg)
{
	while (1)
	{
		printf("foo: %d\n", *(int *)arg);
		usleep(200);
	}
}

void print_err_msg(void)
{
	printf("\033[1;31mInvalid arguments!\n");
	printf("\033[1;33mPlease enter the arguments like below ");
	printf("( Numbers must be greater than zero! )\n");
	printf("\033[0;32mNumber of Philosophers, Time to Die, Time to Eat, ");
	printf("Time to Sleep, \033[1;30m[ Number of Times Each Philosopher ");
	printf("Must Eat ]⌟\033[0;0m\n");
}

long my_gettimeofday(void)
{
	struct timeval	tv;
	long ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ret);
}

void *philo_routine(void* args)
{
	t_philo_stat *philo_stat;

	philo_stat = (t_philo_stat *)args;
	// if (philo_stat->philo_num % 2)
		// usleep(100);

	// for (int i = 0; i < 3; i++)
	while (1)
	{
		// pthread_mutex_lock(&philo_stat->philo_ref->check);

		// if (philo_stat->philo_ref->is_anyone_die)
			// return (NULL);
		printf("I'm philosopher %d\n", philo_stat->philo_num);

		// pthread_mutex_unlock(&philo_stat->philo_ref->check);

		usleep(200);
	}
}

int init_philo(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	int cnt;

	pthread_mutex_init(&philo_ref->check, NULL);
	
	*philo_arr = (t_philo_stat *)malloc(sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	if (*philo_arr == NULL)
		return (0);
	memset(*philo_arr, 0, sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	// while (cnt < 2)
	{
		(*philo_arr)[cnt].philo_num = cnt;
		(*philo_arr)[cnt].philo_ref = philo_ref;
		pthread_create(&(*philo_arr)[cnt].philo_thread, NULL, philo_routine, (void *)&(*philo_arr)[cnt]);
		// pthread_create(&(philo_arr[cnt].philo_thread), NULL, foo, (void *)(&(philo_arr[cnt].philo_num)));
		cnt++;
	}

	// pthread_t my_tr;
	// int *ab = malloc(sizeof(int));
	// *ab = 3;
	// pthread_create(&my_tr, NULL, foo, (void *)ab);
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
	memset(&philo_ref, -1, sizeof(t_philo_ref));

	if (!parse_philo(&philo_ref, argc, argv))
	{
		print_err_msg();
		return (0);
	}

	if (!init_philo(&philo_ref, &philo_arr))
		return (0);
	usleep(200);

	cnt = 0;
	while (cnt < philo_ref.number_of_philosophers)
	{
		pthread_join(philo_arr[cnt].philo_thread, NULL);
		cnt++;
	}

	// -------------

	// pthread_t tr1;
	// int tr1a = 1;
	// pthread_t tr2;
	// int tr2a = 2;
	// pthread_create(&tr1, NULL, foo, (void *)&tr1a);
	// pthread_create(&tr2, NULL, foo, (void *)&tr2a);

	// pthread_join(tr1, NULL);
	// pthread_join(tr2, NULL);

	return (0);
}