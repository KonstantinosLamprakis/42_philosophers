/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 03:13:26 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/09 08:48:58 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO:
// check for mem leaks
// use safe functions ex. write instead of printf
// check for errors wherever I have != 0

// https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/

/*
	Info:
		- pthread_join(): wait for a thread to return.
		If you don't do it and main terminated, the thread continue to exist
		and may have segm fault if use main's resources
		- pthread_detach(): say that this thread has no longer nothing to do
		with main, no resources, not accessible, it runs independently
 */

void	*thread_routine(void *philo);

int	main(int argc, char **argv)
{
	t_info			info_s;
	t_philo			*philo_s;
	int				i;

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
		pthread_create(&(philo_s[i].thread), NULL, thread_routine, (void *)&(philo_s[i]));
	}
	i = -1;
	while (++i < info_s.phil_n)
		pthread_join(philo_s[i].thread, NULL);
	return (0);
}

void	*thread_routine(void *philo)
{
	t_philo	*philo_s;
	static int	count = 0;

	philo_s = (t_philo*)philo;
	pthread_mutex_lock(&(philo_s->info->print_m));
	printf("Thread [%ld]: Count at thread start = %d - %d\n", pthread_self(), philo_s->id, count++);
	pthread_mutex_unlock(&(philo_s->info->print_m));
	return (NULL);
}
