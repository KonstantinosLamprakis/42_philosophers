# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/08 02:28:31 by klamprak          #+#    #+#              #
#    Updated: 2024/04/09 22:05:11 by klamprak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c validation.c philo_utils.c

OBJS = $(SRCS:.c=.o)

NAME = philo

HEADERS = philo.h

all: $(NAME)

%.o: %.c
	cc $(CFLAGS) -o $@ -c $< -I $(HEADERS)

noflags: $(OBJS)
	cc $(OBJS) -I $(HEADERS) -o $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	cc $(CFLAGS) $(OBJS) -I $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
