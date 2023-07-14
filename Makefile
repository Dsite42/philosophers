# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/18 15:39:27 by cgodecke          #+#    #+#              #
#    Updated: 2023/07/14 11:25:43 by cgodecke         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CC_FLAGS = -Wall -Wextra -g #-fsanitize=address #-fsanitize=thread #-Werror
OBJDIR = obj
HEADER := philo.h
INFILES = 	main.c\
			init.c\
			ft_atoi.c\
			philo_thread.c\
			threads.c\

OBJFILES = $(INFILES:%.c=$(OBJDIR)/%.o)

NAME = philo

all: setup $(NAME)

$(NAME): $(OBJFILES) ${HEADER}
	$(CC) $(CC_FLAGS) $(INFILES) -o $(NAME) -pthread

$(OBJDIR)/%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@ -pthread

setup:
	@mkdir -p $(OBJDIR)/obj

clean:
	rm -f $(OBJFILES)
	rm -r -f obj/

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean setup re