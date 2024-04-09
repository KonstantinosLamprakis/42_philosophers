/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 22:04:52 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/09 22:08:11 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// runs constantly until a thread die or all eat the number required
// then they terminate them setting the flag terinate as 1
// if is_dead == -1 don't print neither 'a' or 'b' because is due to
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
	while (!is_eaten && !is_dead)
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
	int		first_fork;
	int		sec_fork;

	philo_s = (t_philo *)philo;
	first_fork = philo_s->id + (philo_s->id % 2 == 1);
	sec_fork = philo_s->id + (philo_s->id % 2 == 0);
	while (!philo_s->info->terminate)
	{
		pthread_mutex_lock(&(philo_s->info->forks[first_fork]));
		print_log('f', philo);
		pthread_mutex_lock(&(philo_s->info->forks[sec_fork]));
		print_log('f', philo);
		print_log('e', philo);
		if (gettimeofday(&(philo_s->last_eat), NULL) != 0)
			return (0);
		usleep(philo_s->info->eat_t);
		pthread_mutex_unlock(&(philo_s->info->forks[sec_fork]));
		pthread_mutex_unlock(&(philo_s->info->forks[first_fork]));
		philo_s->eaten_n++;
		print_log('s', philo);
		usleep(philo_s->info->sleep_t);
		print_log('t', philo);
	}
	return (NULL);
}

// returns a timestamp from time tv_in until now - current time, -1 on error
// usefull to see if philo starved from last time started to eat
// usefull also to print timestamp on logs
long	get_timestamp(struct timeval tv_in)
{
	struct timeval	tv_cur;
	long			timestamp;

	if (gettimeofday(&tv_cur, NULL) != 0)
		return (-1);
	timestamp = (tv_cur.tv_sec - tv_in.tv_sec) \
	* 1000000 + tv_cur.tv_usec - tv_in.tv_usec;
	return (timestamp);
}

// returns 0 in error, 1 in success
// prints the logs
// str is 'f' for fork, 'e' for eating, 's' for sleeping
// 't' for thinking, 'b' for dead and 'a' for all eat enough
// after 'b' or 'a' stops printing anything
// terinate check should be after mutex_lock because otherwise
// may be waiting there and not notice the change on terminate var
int	print_log(char str, t_philo *philo_s)
{
	static int	terminate = 0;
	long		stamp;

	stamp = get_timestamp(philo_s->info->tv_in);
	if (stamp == -1)
		return (0);
	if (pthread_mutex_lock(&(philo_s->info->print_m)) != 0)
		return (0);
	if (terminate)
		return (pthread_mutex_unlock(&(philo_s->info->print_m)) == 0);
	if (str == 'f')
		printf("%ld %d has taken a fork\n", stamp, philo_s->id);
	else if (str == 'e')
		printf("%ld %d is eating\n", stamp, philo_s->id);
	else if (str == 's')
		printf("%ld %d is sleeping\n", stamp, philo_s->id);
	else if (str == 't')
		printf("%ld %d is thinking\n", stamp, philo_s->id);
	else if (str == 'b')
		printf("%ld %d died\nEND\n", stamp, philo_s->id);
	else if (str == 'a')
		printf("%ld Everybody eat %d number of times\nEND\n", \
		stamp, philo_s->info->eat_n);
	terminate = (str == 'a') || (str == 'b');
	return (pthread_mutex_unlock(&(philo_s->info->print_m)) == 0);
}
