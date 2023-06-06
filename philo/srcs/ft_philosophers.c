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

int init_philo(int num_of_philo, int die, int eat, int sleep, int num_of_time)
{
	// pthread_create();
	return (0);
}

void *my_tread_routine(void *args)
{
	while (1)
	{
		printf("Hi! I'm Thread!\n");
		printf("args: %d\n", *((int *)args));
		*(int *)args += 1;
		usleep(5000);
	}
	return 0;
}
void *my_tread_routine2(void *args)
{
	while (1)
	{
		printf("Hi! I'm Thread2!\n");
		printf("args: %d\n", *((int *)args));
		*(int *)args -= 1;
		usleep(5000);
	}
	return 0;
}

// void *test(void *data)
// {
//     int i;
//     int a = *(int *)data;
//     for (i = 0; i < 10; i++)
//     {
//         printf("%d\n", i*a);
//     }
//     return (void *)(i * 100);
// }


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	struct timeval a;
	gettimeofday(&a, NULL);
	printf("now is : %ld\n", a.tv_sec);

	int my_arg = 0;
	pthread_t my_thread;
	pthread_t my_thread2;
	pthread_create(&my_thread, NULL, my_tread_routine, (void *)&my_arg);
	pthread_create(&my_thread2, NULL, my_tread_routine2, (void *)&my_arg);
	pthread_join(my_thread, NULL); // 이게 없으면 스레드의 함수가 진행되기도전에 main의 코드가 끝나서 프로세스가 종료되어버린다
	pthread_join(my_thread2, NULL);

	// init_philo(int num_of_philo, int die, int eat, int sleep, int num_of_time);
	return (0);

	// int a = 100;
    // pthread_t thread_t;
    // int status;

    // // 쓰레드를 생성한다. 
    // if (pthread_create(&thread_t, NULL, test, (void *)&a) < 0)
    // {
    //     perror("thread create error:");
    //     exit(0);
    // }

    // // 쓰레드가 종료되기를 기다린후 
    // // 쓰레드의 리턴값을 출력한다. 
    // pthread_join(thread_t, (void **)&status);
    // printf("Thread End %d\n", status);
    // return 1;
}