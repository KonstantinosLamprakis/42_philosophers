/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 03:13:26 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/09 14:20:38 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO:
// check for mem leaks
// use safe functions ex. write instead of printf
// check for errors wherever I have != 0
// take care of case of single philo on table
// start check_if_die in different process and pass a flag to all threads to
// stop if someone die and also remove print mutex(same for eaten_num)

// https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/

/*
	Info:
		- pthread_join(): wait for a thread to return.
		If you don't do it and main terminated, the thread continue to exist
		and may have segm fault if use main's resources
		- pthread_detach(): say that this thread has no longer nothing to do
		with main, no resources, not accessible, it runs independently

	requipments:
		- eat -> sleep -> think -> eat
		- stops at die or eaten_num all of them
		- message philo died < 10 ms
*/

void	*thread_routine(void *philo);
long	get_timestamp(struct timeval tv_in);
void	print_log(char str, t_philo *philo_s);
void	*monitor_death_eat(void *philo);

int	main(int argc, char **argv)
{
	t_info			info_s;
	t_philo			*philo_s;
	int				i;
	pthread_t		thread;

	if(!check_input(&info_s, argc, argv))
		return (1);
	info_s.forks = malloc (sizeof(pthread_mutex_t) * info_s.phil_n);
	if (!info_s.forks)
		return (1);
	philo_s = malloc (sizeof(t_philo) * info_s.phil_n);
	if (!philo_s)
		return (free(info_s.forks), 1);
	pthread_mutex_init(&(info_s.print_m), NULL);
	i = -1;
	while (++i < info_s.phil_n)
		pthread_mutex_init(&(info_s.forks[i]), NULL);
	i = -1;
	while (++i < info_s.phil_n)
	{
		philo_s[i].eaten_n = 0;
		philo_s[i].id = i;
		philo_s[i].info = &info_s;
		philo_s[i].state = 't';
	}
	if (gettimeofday(&(info_s.tv_in), NULL) != 0)
		return (0);
	i = -1;
	while (++i < info_s.phil_n)
	{
		if (gettimeofday(&(philo_s[i].last_eat), NULL) != 0)
			return (0);
		pthread_create(&(philo_s[i].thread), NULL, thread_routine, (void *)&(philo_s[i]));
	}
	pthread_create(&thread, NULL, monitor_death_eat, (void *)(philo_s));
	i = -1;
	while (++i < info_s.phil_n)
		pthread_join(philo_s[i].thread, NULL);
	pthread_join(thread, NULL);
	return (0);
}

void	*monitor_death_eat(void *philo)
{
	t_philo	*philo_s;
	int	i;
	int	is_eaten;
	int	is_dead;

	is_eaten = 0;
	is_dead = 0;
	philo_s = (t_philo*)philo;
	while (!is_eaten && !is_dead)
	{
		i = -1;
		is_eaten = philo_s[0].info->eat_n != -1;
		while (is_eaten && ++i < philo_s[0].info->phil_n)
		{
			if (philo_s[i].eaten_n < philo_s[0].info->eat_n)
				is_eaten = 0;
		}
		i = -1;
		while (++i < philo_s[0].info->phil_n)
			is_dead += get_timestamp(philo_s[i].last_eat) > philo_s[0].info->die_t;
	}
	if (is_dead)
		print_log('d', &philo_s[0]);
	else if (is_eaten)
		print_log('a', &philo_s[0]);
	philo_s[0].info->terminate = 1;
	return (NULL);
}

// this function runs every time a thread created for a philosopher
// takes as argument a struct which contains all info about this philosopher
void	*thread_routine(void *philo)
{
	t_philo	*philo_s;
	int	first_fork;
	int	sec_fork;

	philo_s = (t_philo*)philo;
	first_fork = philo_s->id;
	sec_fork = philo_s->id;
	if (philo_s->id % 2 == 1)
		first_fork++;
	else
		sec_fork++;
	while(!philo_s->info->terminate)
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

void	print_log(char str, t_philo *philo_s)
{
	static int	terminate = 0;

	pthread_mutex_lock(&(philo_s->info->print_m));
	if (terminate)
	{
		pthread_mutex_unlock(&(philo_s->info->print_m));
		return ;
	}
	if (str == 'f')
		printf("%ld %d has taken a fork\n", get_timestamp(philo_s->info->tv_in), philo_s->id);
	else if (str == 'e')
		printf("%ld %d is eating\n", get_timestamp(philo_s->info->tv_in), philo_s->id);
	else if (str == 's')
		printf("%ld %d is sleeping\n", get_timestamp(philo_s->info->tv_in), philo_s->id);
	else if (str == 't')
		printf("%ld %d is thinking\n", get_timestamp(philo_s->info->tv_in), philo_s->id);
	else if (str == 'd')
	{
		printf("%ld %d died\nEND\n", get_timestamp(philo_s->info->tv_in), philo_s->id);
		terminate = 1;
	}
	else if (str == 'a')
	{
		printf("%ld Everybody eat %d number of times\nEND\n", get_timestamp(philo_s->info->tv_in), philo_s->info->eat_n);
		terminate = 1;
	}
	pthread_mutex_unlock(&(philo_s->info->print_m));
}

// returns a timestamp from time tv_in until now - current time
// usefull to see if philo starved from last time started to eat
// usefull also to print timestamp on logs
long	get_timestamp(struct timeval tv_in)
{
	struct timeval	tv_cur;
	long	timestamp;

	if (gettimeofday(&tv_cur, NULL) != 0)
		return (0);
	timestamp = (tv_cur.tv_sec - tv_in.tv_sec) * 1000000 + tv_cur.tv_usec - tv_in.tv_usec;
	return (timestamp);
}
