/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 15:42:40 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/10 15:26:46 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>

/*
t_info: general info about program
die_t: time from last starting meal until he died from starving
eat_t: how much time needed to eat
sleep_t: how much time needed to sleep
eat_n: optional (-1 if not provided), how many times to eat before exit(0)
phil_n: number of philosophers
tv_in: timeval from the beggining of the program
terminate: means all thread should return, due to died philo or eaten_number
 */
typedef struct s_info
{
	int				die_t;
	int				eat_t;
	int				sleep_t;
	int				eat_n;
	int				phil_n;
	int				terminate;
	int				is_error;
	struct timeval	tv_in;
	pthread_mutex_t	print_m;
	pthread_mutex_t	*forks;
}					t_info;

/*
philo = philosopher
state = 't': thinking, 's': sleeping, 'e': eating
last_eat = timestamp for the last meal he had
eaten_n = how many time has eaten
info = general info of the program
 */
typedef struct s_philo
{
	int				id;
	char			state;
	struct timeval	last_eat;
	int				eaten_n;
	pthread_t		thread;
	t_info			*info;
}					t_philo;

// Declarations of validation.c
int		check_input(t_info *t_ar, int argc, char **argv);
// Declarations of philo_utils.c
void	*thread_routine(void *philo);
void	print_log(char str, t_philo *philo_s);
void	*monitor_death_eat(void *philo);
// Declarations of philo_utils2.c
int		ft_usleep(size_t milliseconds);
// Declarations of main.c
long	get_timestamp(struct timeval tv_in);

#endif
