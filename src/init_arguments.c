/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_arguments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 16:50:24 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/17 17:20:58 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int	is_valid_arguments(t_state *state)
{
	if (state->number_of_philosophers <= 0 || state->time_to_die <= 0
		|| state->time_to_eat <= 0 || state->time_to_sleep <= 0
		|| state->number_of_times_each_philosopher_must_eat < 0)
	{
		printf("No valid input. Please insert positive integers.\n");
		return (-1);
	}
	return (0);
}

int	init_arguments(t_state *state, int argc, char **argv)
{
	(*state).number_of_philosophers = ft_atoi(argv[1]);
	(*state).time_to_die = ft_atoi(argv[2]);
	(*state).time_to_eat = ft_atoi(argv[3]);
	(*state).time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		(*state).number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		(*state).number_of_times_each_philosopher_must_eat = 0;
	if (is_valid_arguments(state) == -1)
		return (-1);
	return (0);
}
