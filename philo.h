/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 15:42:40 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/08 13:05:05 by klamprak         ###   ########.fr       */
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
 */
typedef struct	s_info
{
	int	die_t;
	int	eat_t;
	int	sleep_t;
	int	eat_n;
	int	phil_n;
	struct timeval	tv_in;
	pthread_mutex_t	print_m;
	pthread_mutex_t	*forks;
}		t_info;

/*
philo = philosopher
state = 't': thinking, 's': sleeping, 'e': eating
last_eat = timestamp for the last meal he had
eaten_n = how many time has eaten
info = general info of the program
 */
typedef struct	s_philo
{
	int			id;
	char		state;
	long		last_eat;
	int			eaten_n;
	t_info		info;
}				t_philo;

// Declarations of validation.c
int	check_input(t_info *t_ar, int argc, char **argv);

#endif
