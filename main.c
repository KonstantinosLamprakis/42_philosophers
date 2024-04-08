/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 03:13:26 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/08 11:51:09 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO:
// check for mem leaks
// use safe functions ex. write instead of printf
// check for errors wherever I have != 0

// https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/

int	main(int argc, char **argv)
{
	if(!check_input(&s_args, argc, argv))
		return (1);
}
