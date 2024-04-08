/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 03:13:26 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/08 05:02:38 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO:
// remove stdio.h from header
// check for mem leaks

// https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/

void	*trythis(void *arg){
	arg = NULL;
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_args			s_args;
	pthread_t		*threads;
	pthread_mutex_t	lock;
	int				i;
	struct timeval	tv_in;
	struct timeval	tv_cur;
	long			timestamp;

	gettimeofday(&tv_in, NULL);
	if(!check_input(&s_args, argc, argv))
		return (1);
	threads = malloc (s_args.phil_n * sizeof(pthread_t *));
	if (!threads)
		return (1);
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		free(threads);
		return (1);
	}
	i = -1;
	while (++i < s_args.phil_n)
		if (pthread_create(&(threads[i]), NULL, &trythis, NULL) != 0)
		{
			free(threads);
			return (1);
		}
	gettimeofday(&tv_cur, NULL);
	timestamp = (tv_cur.tv_sec - tv_in.tv_sec) * 1000000 + tv_cur.tv_usec - tv_in.tv_usec;
}
