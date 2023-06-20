/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosophers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 12:30:01 by yonghyle          #+#    #+#             */
/*   Updated: 2023/06/02 12:45:15 by yonghyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILOSOPHERS_H
# define FT_PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_philo_stat
{
	struct s_philo_ref	*philo_ref;
	pthread_t			philo_thread;
	int					philo_num;
	int					fork_idx;
	int					*fork[2];
	pthread_mutex_t		*m_fork[2];
	int					cur_state;
	int					how_much_eat;
	long				last_time_to_eat;
	long				last_time_to_sleep;
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
	pthread_mutex_t	m_die;
	pthread_mutex_t	m_full_eat;
	pthread_mutex_t	*m_fork_arr;
	void			(*state_func[3])(struct s_philo_stat *);
	int				*fork_arr;
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
long	my_gettimeofday(void);
int		parse_philo(t_philo_ref *philo_ref, int argc, char **argv);

long	get_sleep_time(t_philo_stat *philo_stat);
void	philo_think(t_philo_stat *philo_stat);
void	philo_eat(t_philo_stat *philo_stat);
void	philo_sleep(t_philo_stat *philo_stat);

#endif