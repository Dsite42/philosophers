# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/18 15:39:27 by cgodecke          #+#    #+#              #
#    Updated: 2023/07/26 16:22:54 by cgodecke         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CC_FLAGS = -Wall -Wextra -Werror #-g -fsanitize=thread
OBJDIR = obj
HEADER := src/philo.h
INFILES = 	src/main.c\
			src/init.c\
			src/init_arguments.c\
			src/fill_state.c\
			src/ft_atoi.c\
			src/ft_wait.c\
			src/philo_thread.c\
			src/threads.c\
			src/is_program_end.c\
			src/print_state_change.c\
			src/forks.c\
			src/dead.c\
			src/thinking.c\


OBJFILES = $(INFILES:%.c=$(OBJDIR)/%.o)

NAME = philo

all: setup $(NAME)

$(NAME): $(OBJFILES) ${HEADER}
	$(CC) $(CC_FLAGS) $(INFILES) -o $(NAME) -pthread

$(OBJDIR)/%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@ -pthread

setup:
	@mkdir -p $(OBJDIR)/src

clean:
	rm -f $(OBJFILES)
	rm -r -f obj/

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean setup re