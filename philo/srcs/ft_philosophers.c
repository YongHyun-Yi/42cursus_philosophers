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

int init_philo(int num_of_philo, int die, int eat, int sleep, int num_of_time)
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
	struct timeval a;
	gettimeofday(&a, NULL);
	printf("now is : %ld\n", a.tv_sec);

	pthread_mutex_init(&my_mutex, NULL);

	pthread_t my_thread;
	pthread_t my_thread2;
	pthread_create(&my_thread, NULL, my_tread_routine, (void *)&my_var);
	pthread_create(&my_thread2, NULL, my_tread_routine2, (void *)&my_var);

	pthread_t pt_arr[2];
	pt_arr[0] = my_thread;
	pt_arr[1] = my_thread2;

	for (int i = 0; i < 2; i++)
		pthread_join(pt_arr[i], NULL);

	// init_philo(int num_of_philo, int die, int eat, int sleep, int num_of_time);
	printf("my_var: %d\n", my_var);
	return (0);
}