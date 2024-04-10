/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 22:04:52 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/10 10:50:44 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_str(char str, long stamp, t_philo *philo_s);
static void	*loop_routine(t_philo *philo_s, int fork1, int fork2);

// runs constantly until a thread die or all eat the number required
// then they terminate them setting the flag terinate as 1
// if is_dead == -1 don't print neither 'a' or 'b' because is due to -
// error on get_timestamp, will just terminate the program
void	*monitor_death_eat(void *philo)
{
	t_philo	*philo_s;
	int		i;
	int		is_eaten;
	int		is_dead;

	is_eaten = 0;
	is_dead = 0;
	philo_s = (t_philo *)philo;
	while (!is_eaten && !is_dead && !philo_s[0].info->is_error)
	{
		i = -1;
		is_eaten = philo_s[0].info->eat_n != -1;
		while (is_eaten && ++i < philo_s[0].info->phil_n)
			if (philo_s[i].eaten_n < philo_s[0].info->eat_n)
				is_eaten = 0;
		i = 0;
		while (!is_dead && i < philo_s[0].info->phil_n)
			is_dead += get_timestamp(philo_s[i++].last_eat) \
			> philo_s[0].info->die_t;
	}
	if (is_dead > 0 || is_eaten)
		print_log('a' + (is_dead > 0), &philo_s[--i]);
	philo_s[0].info->terminate = 1;
	philo_s[0].info->is_error += is_dead == -1;
	return (NULL);
}

// this function runs every time a thread created for a philosopher
// takes as argument a struct which contains all info about this philosopher
// every even thread takes left fork first, right second
// every odd thread takes right fork first, left second
// this way never have a deadlock
void	*thread_routine(void *philo)
{
	t_philo	*philo_s;
	int		fork1;
	int		fork2;

	philo_s = (t_philo *)philo;
	fork1 = philo_s->id + (philo_s->id % 2 == 1);
	if (philo_s->id == philo_s->info->phil_n - 1)
		fork1 = 0;
	fork2 = philo_s->id + (philo_s->id % 2 == 0);
	while (!philo_s->info->terminate && !philo_s->info->is_error)
	{
		loop_routine(philo_s, fork1, fork2);
		if (philo_s->info->phil_n == 1)
			break ;
	}
	return (NULL);
}

static void	*loop_routine(t_philo *philo_s, int fork1, int fork2)
{
	if (pthread_mutex_lock(&(philo_s->info->forks[fork1])) != 0)
		return (philo_s->info->is_error = 1, NULL);
	print_log('f', philo_s);
	if (philo_s->info->phil_n == 1)
		return (pthread_mutex_unlock(&(philo_s->info->forks[fork1])), NULL);
	if (pthread_mutex_lock(&(philo_s->info->forks[fork2])) != 0)
	{
		pthread_mutex_unlock(&(philo_s->info->forks[fork1]));
		return (philo_s->info->is_error = 1, NULL);
	}
	print_log('e', philo_s);
	if (gettimeofday(&(philo_s->last_eat), NULL) != 0)
	{
		pthread_mutex_unlock(&(philo_s->info->forks[fork2]));
		pthread_mutex_unlock(&(philo_s->info->forks[fork1]));
		return (philo_s->info->is_error = 1, NULL);
	}
	usleep(philo_s->info->eat_t);
	pthread_mutex_unlock(&(philo_s->info->forks[fork2]));
	pthread_mutex_unlock(&(philo_s->info->forks[fork1]));
	philo_s->eaten_n++;
	print_log('s', philo_s);
	usleep(philo_s->info->sleep_t);
	print_log('t', philo_s);
	return (NULL);
}

// prints the logs
// str is 'f' for fork, 'e' for eating(and grab 2nd fork), 's' for sleeping
// 't' for thinking, 'b' for dead and 'a' for all eat enough
// after 'b' or 'a' stops printing anything
// terinate check should be after mutex_lock because otherwise -
// may be waiting there and not notice the change on terminate var
// update error and terminate variable as needed
void	print_log(char str, t_philo *philo_s)
{
	static int	terminate = 0;
	long		stamp;

	if (pthread_mutex_lock(&(philo_s->info->print_m)) != 0)
	{
		philo_s->info->is_error = 1;
		return ;
	}
	stamp = get_timestamp(philo_s->info->tv_in);
	if (stamp == -1)
	{
		philo_s->info->is_error = 1;
		pthread_mutex_unlock(&(philo_s->info->print_m));
		return ;
	}
	if (terminate)
	{
		pthread_mutex_unlock(&(philo_s->info->print_m));
		return ;
	}
	print_str(str, stamp, philo_s);
	terminate = (str == 'a') || (str == 'b');
	if (pthread_mutex_unlock(&(philo_s->info->print_m)) != 0)
		philo_s->info->is_error = 1;
}

static void	print_str(char str, long stamp, t_philo *philo_s)
{
	if (str == 'f')
		printf("%ld %d has taken a fork\n", stamp, philo_s->id);
	else if (str == 'e')
		printf("%ld %d has taken second fork\n%ld %d is eating\n" \
		, stamp, philo_s->id, stamp, philo_s->id);
	else if (str == 's')
		printf("%ld %d is sleeping\n", stamp, philo_s->id);
	else if (str == 't')
		printf("%ld %d is thinking\n", stamp, philo_s->id);
	else if (str == 'b')
		printf("%ld %d died\nEND\n", stamp, philo_s->id);
	else if (str == 'a')
		printf("%ld Everybody eat %d number of times\nEND\n", \
		stamp, philo_s->info->eat_n);
}
