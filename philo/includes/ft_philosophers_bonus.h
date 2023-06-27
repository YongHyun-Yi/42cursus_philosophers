/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers_bonus.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:30:01 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/27 10:58:21 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_BONUS_H
# define FT_PHILOSOPHERS_BONUS_H

#include <fcntl.h>

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_philo_stat
{
	struct s_philo_ref	*philo_ref;
	pthread_t			philo_thread;
	int					philo_num;
	int					cur_state;
	int					how_much_eat;
	long				last_time_to_eat;
	long				last_time_to_sleep;
	char				*s_die_name;
	sem_t				*s_die;
}	t_philo_stat;

typedef struct s_philo_ref
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_must_eat;
	int				number_of_full_philosophers;
	int				is_anyone_die;
	sem_t			*s_full_eat;
	sem_t			*s_fork;
	long			start_time;
}	t_philo_ref;

typedef enum e_philo_state
{
	THINK,
	EAT,
	SLEEP
}	t_philo_state;

int		ft_strisnum(char *str);
int		ft_atoi(const char *str);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);
char	*ft_itoa(int n);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *str);

long	my_gettimeofday(void);
void	print_philo(t_philo_stat *philo_stat, char *state);
long	get_sleep_time(t_philo_stat *philo_stat);

int		parse_philo(t_philo_ref *philo_ref, int argc, char **argv);

int		init_philo(t_philo_ref *philo_ref, t_philo_stat *philo_arr);

void	*philo_routine(void *args);
// void	*philo_routine(t_philo_stat	*philo_stat);

void	philo_think(t_philo_stat *philo_stat);
void	philo_eat(t_philo_stat *philo_stat);
void	philo_sleep(t_philo_stat *philo_stat);

#endif