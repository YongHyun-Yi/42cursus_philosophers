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
	// while (1)
	// {
	// 	pthread_mutex_lock(&my_mutex);
	// 	printf("Hi! I'm Thread!\n");
	// 	printf("args: %d\n", *((int *)args));
	// 	*(int *)args += 1;
	// 	pthread_mutex_unlock(&my_mutex);
	// 	usleep(1000 * 100);
	// 	// sleep(1);
	// }
	for (int i = 0; i < 50000; i++)
	{
		// printf("my arg: %d\n", *(int *)args);
		// *(int *)args += 1;
		my_var++;
		// usleep(100);
	}
	// {
		// pthread_mutex_lock(&my_mutex);
		// *(int *)args += 1;
		// usleep(100);
		// pthread_mutex_unlock(&my_mutex);
	// }
	return 0;
}
void *my_tread_routine2(void *args)
{
	// while (1)
	// {
	// 	pthread_mutex_lock(&my_mutex);
	// 	printf("Hi! I'm Thread2!\n");
	// 	printf("args: %d\n", *((int *)args));
	// 	*(int *)args -= 1;
	// 	pthread_mutex_unlock(&my_mutex);
	// 	usleep(1000 * 100);
	// 	// sleep(1);
	// 	// pthread_exit(NULL); // exit 를 쓰면 프로세스가 종료되어버린다 pthread_exit 는 스레드만 종료시킨다
	// }
	for (int i = 0; i < 50000; i++)
	{
		// printf("my arg: %d\n", *(int *)args);
		*(int *)args -= 1;
		// usleep(100);
	}
	// {
		// pthread_mutex_lock(&my_mutex);
		// *(int *)args -= 1;
		// usleep(100);
		// pthread_mutex_unlock(&my_mutex);
	// }
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

	int my_arg = 0;
	// int *my_var = malloc(sizeof(int));
	// *my_var = 0;
	pthread_t my_thread;
	pthread_t my_thread2;
	pthread_create(&my_thread, NULL, my_tread_routine, (void *)&my_var);
	// usleep(500);
	pthread_create(&my_thread2, NULL, my_tread_routine2, (void *)&my_var);

	pthread_t pt_arr[2];
	pt_arr[0] = my_thread;
	pt_arr[1] = my_thread2;


	for (int i = 0; i < 2; i++)
		pthread_join(pt_arr[i], NULL);
	// pthread_join(pt_arr[0], NULL);
	// pthread_join(pt_arr[1], NULL);
	/*
		join 을 미리 써놓고 스레드의 반환값을 기다리는게 아닌거같음
		for 문에서 돌려버려도 그전에 끝나있으면 알아서 인식하는듯
		for 문으로 한번에 써버리는게 줄 수도 줄고 관리도 좋은것같음
	*/

	// pthread_join(my_thread, NULL); // 이게 없으면 스레드의 함수가 진행되기도전에 main의 코드가 끝나서 프로세스가 종료되어버린다
	// pthread_join(my_thread2, NULL);

	// init_philo(int num_of_philo, int die, int eat, int sleep, int num_of_time);
	printf("my_var: %d\n", my_var);
	return (0);
}