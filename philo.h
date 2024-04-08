/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 15:42:40 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/08 03:38:56 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
// Custom declarations
# define BUFFER_SIZE 42

// t = time
// n = number
// s_args = arguments of main program
typedef struct s_args
{
	int	die_t;
	int	eat_t;
	int	sleep_t;
	int	eat_n;
	int	phil_n;
}		t_args;

// Declarations of validation.c
int	check_input(t_args *t_ar, int argc, char **argv);

#endif
