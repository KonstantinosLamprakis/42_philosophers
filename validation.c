/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 03:21:39 by klamprak          #+#    #+#             */
/*   Updated: 2024/04/09 11:01:02 by klamprak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_int(char *str, int *result);
static int	ft_atoi(const char *str);

// returns 0 in error, 1 in success and puts the result on t_ar
// eat_n = -1 means there no argument for number of eating
// validation: 5 or 6 number of args, all of them positive ints
int	check_input(t_info *t_ar, int argc, char **argv)
{
	int	err;

	err = 0;
	if (argc != 5 && argc != 6)
	{
		write(1, "Wrong Args\n", 11);
		return (0);
	}
	err += !is_int(argv[1], &t_ar->phil_n) || !is_int(argv[2], &t_ar->die_t);
	err += !is_int(argv[3], &t_ar->eat_n) || !is_int(argv[4], &t_ar->sleep_t);
	if (argc == 6)
		err +=  !is_int(argv[5], &t_ar->eat_n);
	else
		t_ar->eat_n = -1;
	if (err)
		write(1, "Wrong Args\n", 11);
	t_ar->terminate = 0;
	return (!err);
}

static int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	div;
	long	result;
	int		end;

	i = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '+') * 2 - 1;
	result = 0;
	end = i;
	while (str[end] != '\0')
		end++;
	div = 1;
	while (str[end - 1] >= '0' && str[end - 1] <= '9')
	{
		result = result - ((str[end - 1] - '0') * div);
		div *= 10;
		if (end == 1)
			break ;
		end--;
	}
	return (result * sign * -1);
}

// insteger means less than 10 digits + 1 for sign
// checks also for overflow, underflow 2147483647 -2147483648
// checks if exist characters other than digits
// returns 1 if number is positive integer or 0 if not
// puts the result number on &result
static int	is_int(char *str, int *result)
{
	int		i;
	long	temp;

	i = 0;
	if (str[i] == '-')
		return (0);
	if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (i > 11 || i == 0 || (i == 1 && (str[0] == '-' || str[0] == '+')))
		return (0);
	temp = ft_atoi(str);
	if (temp > 2147483647 || temp < -2147483648)
		return (0);
	*result = (int) temp;
	return (1);
}
