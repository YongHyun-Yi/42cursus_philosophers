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

void print_philo(t_philo_stat *philo_stat, long time, char *state)
{
	long print_time;

	print_time = time - philo_stat->philo_ref->start_time;
	printf("%ld %d %s\n", print_time, philo_stat->philo_num, state);
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

void *philo_routine(void* args)
{
	t_philo_stat *philo_stat;

	philo_stat = (t_philo_stat *)args;
	// printf("I'm philosopher %d\n", philo_stat->philo_num);

	if (philo_stat->philo_num % 2)
		usleep(100);
	// pthread_mutex_lock(&philo_stat->philo_ref->m_fork[philo_stat->philo_num]);
	// pthread_mutex_unlock(&philo_stat->philo_ref->m_fork[philo_stat->philo_num]);
	while (1)
	{
		

		if (philo_stat->philo_ref->number_of_times_must_eat == philo_stat->how_much_eat)
		{
			// pthread_mutex_unlock(&philo_stat->philo_ref->check);
			return (NULL);
		}

		pthread_mutex_lock(&philo_stat->philo_ref->check);

		if (philo_stat->philo_ref->is_anyone_die)
		{
			pthread_mutex_unlock(&philo_stat->philo_ref->check);
			return (NULL);
		}
		
		long cmp_time = my_gettimeofday();

		if (cmp_time - philo_stat->last_time_to_eat > philo_stat->philo_ref->time_to_die)
		{
			philo_stat->philo_ref->is_anyone_die = 1;
			// printf("philosopher %d Die...\n", philo_stat->philo_num);
			print_philo(philo_stat, my_gettimeofday(), "died");
			pthread_mutex_unlock(&philo_stat->philo_ref->check);
			return (NULL);
		}

		pthread_mutex_unlock(&philo_stat->philo_ref->check);

		if (philo_stat->cur_state == THINK)
		{
			pthread_mutex_lock(&philo_stat->philo_ref->m_fork[philo_stat->philo_num]);

			if (philo_stat->philo_ref->fork_arr[philo_stat->philo_num] == 0)
			{
				print_philo(philo_stat, my_gettimeofday(), "taking fork1");

				if (philo_stat->philo_num == philo_stat->philo_ref->number_of_philosophers - 1)
				{
					pthread_mutex_lock(&philo_stat->philo_ref->m_fork[0]);

					if (philo_stat->philo_ref->fork_arr[0] == 0)
					{
						philo_stat->philo_ref->fork_arr[philo_stat->philo_num] = 1;
						philo_stat->philo_ref->fork_arr[0] = 1;
						philo_stat->cur_state = EAT;
						philo_stat->last_time_to_eat = my_gettimeofday();
						// printf("philosopher %d Eating...\n", philo_stat->philo_num);
						print_philo(philo_stat, my_gettimeofday(), "taking fork2");
						print_philo(philo_stat, philo_stat->last_time_to_eat, "is eating");
					}

					pthread_mutex_unlock(&philo_stat->philo_ref->m_fork[0]);
				}
				else
				{
					pthread_mutex_lock(&philo_stat->philo_ref->m_fork[philo_stat->philo_num + 1]);

					if (philo_stat->philo_ref->fork_arr[philo_stat->philo_num + 1] == 0)
					{
						philo_stat->philo_ref->fork_arr[philo_stat->philo_num] = 1;
						philo_stat->philo_ref->fork_arr[philo_stat->philo_num + 1] = 1;
						philo_stat->cur_state = EAT;
						philo_stat->last_time_to_eat = my_gettimeofday();
						// printf("philosopher %d Eating...\n", philo_stat->philo_num);
						print_philo(philo_stat, my_gettimeofday(), "taking fork2");
						print_philo(philo_stat, philo_stat->last_time_to_eat, "is eating");
					}

					pthread_mutex_unlock(&philo_stat->philo_ref->m_fork[philo_stat->philo_num + 1]);
				}
			}

			pthread_mutex_unlock(&philo_stat->philo_ref->m_fork[philo_stat->philo_num]);
		}

		else if (philo_stat->cur_state == EAT)
		{
			cmp_time = my_gettimeofday();

			if (cmp_time - philo_stat->last_time_to_eat >= philo_stat->philo_ref->time_to_eat) // 다 먹었을 때
			{
				philo_stat->philo_ref->fork_arr[philo_stat->philo_num] = 0;
				if (philo_stat->philo_num == philo_stat->philo_ref->number_of_philosophers - 1)
					philo_stat->philo_ref->fork_arr[0] = 0;
				else
					philo_stat->philo_ref->fork_arr[philo_stat->philo_num + 1] = 0;

				if (philo_stat->philo_ref->number_of_times_must_eat != -1)
				{
					philo_stat->how_much_eat += 1;
					if (philo_stat->philo_ref->number_of_times_must_eat == philo_stat->how_much_eat)
					{
						// pthread_mutex_unlock(&philo_stat->philo_ref->check);
						return (NULL);
					}
				}
				
				philo_stat->cur_state = SLEEP;
				philo_stat->last_time_to_sleep = my_gettimeofday();
				// printf("philosopher %d Sleeping...\n", philo_stat->philo_num);
				print_philo(philo_stat, philo_stat->last_time_to_sleep, "is sleeping");
			}
		}

		else // sleep
		{
			cmp_time = my_gettimeofday();

			if (cmp_time - philo_stat->last_time_to_sleep >= philo_stat->philo_ref->time_to_sleep)
			{
				philo_stat->cur_state = THINK;
				// printf("philosopher %d Thinking...\n", philo_stat->philo_num);
				print_philo(philo_stat, my_gettimeofday(), "is thinking");
			}
		}

		// pthread_mutex_unlock(&philo_stat->philo_ref->check);

		usleep(100);
	}
}

int init_philo(t_philo_ref *philo_ref, t_philo_stat **philo_arr)
{
	int cnt;
	
	philo_ref->start_time = my_gettimeofday();
	// pthread_mutex_init(&philo_ref->check, NULL);

	philo_ref->m_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * philo_ref->number_of_philosophers);
	if (philo_ref->m_fork == NULL)
		return (0);
	
	for (int i = 0; i < philo_ref->number_of_philosophers; i++)
		pthread_mutex_init(&philo_ref->m_fork[i], NULL);
	
	*philo_arr = (t_philo_stat *)malloc(sizeof(t_philo_stat) * philo_ref->number_of_philosophers);
	if (*philo_arr == NULL)
		return (0);
	memset(*philo_arr, 0, sizeof(t_philo_stat) * philo_ref->number_of_philosophers);

	philo_ref->fork_arr = malloc(sizeof(int) * philo_ref->number_of_philosophers);
	if (philo_ref->fork_arr == NULL)
		return (0);
	memset(philo_ref->fork_arr, 0, sizeof(int) * philo_ref->number_of_philosophers);
	
	cnt = 0;
	while (cnt < philo_ref->number_of_philosophers)
	{
		(*philo_arr)[cnt].philo_num = cnt;
		(*philo_arr)[cnt].last_time_to_eat = philo_ref->start_time;
		(*philo_arr)[cnt].philo_ref = philo_ref;
		pthread_create(&(*philo_arr)[cnt].philo_thread, NULL, philo_routine, (void *)&(*philo_arr)[cnt]);
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

	if (!init_philo(&philo_ref, &philo_arr))
		return (0);

	cnt = 0;
	while (cnt < philo_ref.number_of_philosophers)
	{
		pthread_join(philo_arr[cnt].philo_thread, NULL);
		cnt++;
	}

	return (0);
}