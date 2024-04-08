/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 03:13:26 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/08 04:48:16 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO:
// remove stdio.h from header
// check for mem leaks

// https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/

int	main(int argc, char **argv)
{
	t_args			s_args;
	pthread_t		*threads;
	pthread_mutex_t	lock;
	int				i;

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
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);

}


pthread_t tid[2];
int counter;
pthread_mutex_t lock;

void* trythis(void* arg)
{
    pthread_mutex_lock(&lock);

    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d has started\n", counter);

    for (i = 0; i < (0xFFFFFFFF); i++)
        ;

    printf("\n Job %d has finished\n", counter);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void)
{
    int i = 0;
    int error;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    while (i < 2) {
        error = pthread_create(&(tid[i]),
                               NULL,
                               &trythis, NULL);
        if (error != 0)
            printf("\nThread can't be created :[%s]",
                   strerror(error));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}
