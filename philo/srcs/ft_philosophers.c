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

pthread_mutex_t my_mutex;
int my_var = 0;

void print_err_msg(void)
{
	printf("\033[1;31mInvalid arguments!\n");
	printf("\033[1;33mPlease enter the arguments like below ");
	printf("( Numbers must be greater than zero! )\n");
	printf("\033[0;32mNumber of Philosophers, Time to Die, Time to Eat, ");
	printf("Time to Sleep, \033[1;30m[ Number of Times Each Philosopher ");
	printf("Must Eat ]⌟\033[0;0m\n");
}

int parse_philo(t_philo_ref *philo_ref, char **argv)
{
	if (!ft_strisnum(argv[1]) || !ft_strisnum(argv[2])\
	|| !ft_strisnum(argv[3]) || !ft_strisnum(argv[4]))
	{
		print_err_msg();
		return (0);
	}
	philo_ref->number_of_philosophers = ft_atoi(argv[1]);
	philo_ref->time_to_die = ft_atoi(argv[2]);
	philo_ref->time_to_eat = ft_atoi(argv[3]);
	philo_ref->time_to_sleep = ft_atoi(argv[4]);
	if (philo_ref->number_of_philosophers <= 0 || philo_ref->time_to_die <= 0\
	|| philo_ref->time_to_eat <= 0 || philo_ref->time_to_sleep <= 0)
	{
		print_err_msg();
		return (0);
	}
	return (1);
}

int init_philo(t_philo_ref philo_ref)
{
	// pthread_create();
	return (0);
}

void *my_tread_routine(void *args)
{
	for (int i = 0; i < 50000; i++)
	{
		pthread_mutex_lock(&my_mutex);
        my_var++;
		pthread_mutex_unlock(&my_mutex);
	}
	return 0;
}
void *my_tread_routine2(void *args)
{
	for (int i = 0; i < 25000; i++)
	{
		pthread_mutex_lock(&my_mutex);
        my_var--;
		pthread_mutex_unlock(&my_mutex);
	}
	return 0;
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	// int *my_arr = malloc(sizeof(int) * 5);
	// printf("my_arr: %p\n", my_arr);
	// printf("my_arr[0]: %p\n", &my_arr[0]);
	// printf("my_arr[3]: %p\n", &my_arr[3]);
	// struct timeval a;
	// gettimeofday(&a, NULL);
	// printf("now is : %ld\n", a.tv_sec);

	// pthread_mutex_init(&my_mutex, NULL);

	// pthread_t my_thread;
	// pthread_t my_thread2;
	// pthread_create(&my_thread, NULL, my_tread_routine, (void *)&my_var);
	// pthread_create(&my_thread2, NULL, my_tread_routine2, (void *)&my_var);

	// pthread_t pt_arr[2];
	// pt_arr[0] = my_thread;
	// pt_arr[1] = my_thread2;

	// for (int i = 0; i < 2; i++)
	// 	pthread_join(pt_arr[i], NULL);
	// printf("my_var: %d\n", my_var);

	t_philo_ref philo_ref;
	memset(&philo_ref, -1, sizeof(t_philo_ref));

	if (argc < 5 || argc > 6)
	{
		print_err_msg();
		return (0);
	}
	if (!parse_philo(&philo_ref, argv))
		return (0);
	if (argc == 6)
	{
		if (!ft_strisnum(argv[5]) || argv[5] <= 0)
		{
			print_err_msg();
			return (0);
		}
		philo_ref.time_to_sleep = ft_atoi(argv[5]);
	}
	// init_philo(philo_ref);
	return (0);
}