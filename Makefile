# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: klamprak <klamprak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/08 02:28:31 by klamprak          #+#    #+#              #
#    Updated: 2024/04/08 03:43:40 by klamprak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c validation.c

OBJS = $(SRCS:.c=.o)

NAME = philo

HEADERS = philo.h

all: $(NAME)

%.o: %.c
	cc $(CFLAGS) -o $@ -c $< -I $(HEADERS)

$(NAME): $(OBJS) $(HEADERS)
	cc $(CFLAGS) $(OBJS) -I $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re
