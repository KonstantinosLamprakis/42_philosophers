/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 03:13:26 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/10 15:37:42 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO:
// check for mem leaks

// https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/

/*
	Info:
		- pthread_join(): wait for a thread to return.
		If you don't do it and main terminated, the thread continue to exist
		and may have segm fault if use main's resources
		- pthread_detach(): say that this thread has no longer nothing to do
		with main, no resources, not accessible, it runs independently

	./philo 5 800 200 200
	./philo 5 600 150 150
	./philo 4 410 200 200
	./philo 100 800 200 200
*/

int	clean(int end_mutex, t_philo *philo, t_info info, int end_threads);
int	init_struct(int argc, char **argv, t_info *info_s, t_philo **philo_s);
int	get_struct(int argc, char **argv, t_info *info_s, t_philo **philo_s);

// args: number of philosophers, time to die, time to eat, time to sleep
// optional arg: how many times to eat
int	main(int argc, char **argv)
{
	t_info			info_s;
	t_philo			*philo_s;
	int				i;
	pthread_t		thread;

	i = get_struct(argc, argv, &info_s, &philo_s);
	if (!i)
		return (1);
	i = -1;
	while (++i < info_s.phil_n)
		if ((gettimeofday(&(philo_s[i].last_eat), NULL) != 0) || \
		((pthread_create(&(philo_s[i].thread), NULL, thread_routine, \
		(void *)&(philo_s[i])) != 0)))
			return (clean(info_s.phil_n, philo_s, info_s, i));
	if (pthread_create(&thread, NULL, monitor_death_eat, \
	(void *)(philo_s)) != 0)
		return (clean(info_s.phil_n, philo_s, info_s, info_s.phil_n));
	i = -1;
	while (++i < info_s.phil_n)
		pthread_join(philo_s[i].thread, NULL);
	pthread_join(thread, NULL);
	if (info_s.is_error)
		printf("\nError occured 1: %d\n", info_s.is_error);
	clean(info_s.phil_n, philo_s, info_s, -1);
	return (0);
}

// returns 0 on error, 1 on success
// no need for caller to free anything, it deals with everything
int	get_struct(int argc, char **argv, t_info *info_s, t_philo **philo_s)
{
	int	i;

	i = init_struct(argc, argv, info_s, philo_s);
	if (i != -1)
	{
		clean(i, *philo_s, *info_s, 0);
		return (0);
	}
	while (++i < info_s->phil_n)
	{
		(*philo_s)[i].eaten_n = 0;
		(*philo_s)[i].id = i;
		(*philo_s)[i].info = info_s;
		(*philo_s)[i].state = 't';
	}
	if (gettimeofday(&(info_s->tv_in), NULL) != 0)
	{
		clean(info_s->phil_n, *philo_s, *info_s, 0);
		return (0);
	}
	return (1);
}

// returns
// -1 on success
// -2 on error with no need to free
// i on error with need to free until ith mutex(without it)
// also need to free one mutex for print, and structs(forks, philo)
int	init_struct(int argc, char **argv, t_info *info_s, t_philo **philo_s)
{
	int	i;

	if (!check_input(info_s, argc, argv))
		return (-2);
	info_s->forks = malloc (sizeof(pthread_mutex_t) * info_s->phil_n);
	if (!info_s->forks)
		return (-2);
	*philo_s = malloc (sizeof(t_philo) * info_s->phil_n);
	if (!(*philo_s))
		return (free(info_s->forks), -2);
	if (pthread_mutex_init(&(info_s->print_m), NULL) != 0)
	{
		free(*philo_s);
		return (free(info_s->forks), -2);
	}
	i = -1;
	while (++i < info_s->phil_n)
		if (pthread_mutex_init(&(info_s->forks[i]), NULL) != 0)
			return (i);
	return (-1);
}

// end is the respectable i from init()
// end = -2: nothing to free, end > 0 should free mutex until end
// end_threads: how many threads should wait to join before cleaning
// end_threads = -1 means no threads, no errors
// end_threads = 0 means no threads, but error occured
// end_threads > 0 means should wait for first ith threads, and also had error
int	clean(int end_mutex, t_philo *philo, t_info info, int end_threads)
{
	int	i;

	if (end_threads == -1)
		end_threads = 0;
	else
		printf("Error occured 2\n");
	printf("Cleaning\n");
	if (end_threads > 0)
	{
		info.terminate = 1;
		i = -1;
		while (++i < end_threads)
			pthread_join(philo[i].thread, NULL);
	}
	if (end_mutex == -2)
		return (0);
	i = -1;
	while (++i < end_mutex)
		pthread_mutex_destroy(&(info.forks[i]));
	pthread_mutex_destroy(&(info.print_m));
	free(info.forks);
	free(philo);
	return (0);
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
	timestamp = ((tv_cur.tv_sec - tv_in.tv_sec) \
	* 1000) + ((tv_cur.tv_usec - tv_in.tv_usec) / 1000);
	return (timestamp);
}
