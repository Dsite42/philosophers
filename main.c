/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/06/30 11:09:28 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>

int	is_valid_arguments(t_state *state)
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

int	init_structs(t_state *state)
{
	int	i;

	(*state).p_philosophers = (t_philosopher *)
		malloc(state->number_of_philosophers * sizeof(t_philosopher));
	if ((*state).p_philosophers == NULL)
	{
		printf("malloc of p_philosophers failed.\n");
		return (-1);
	}
	(*state).p_forks = (t_fork *)
		malloc(state->number_of_philosophers * sizeof(t_fork));
	if ((*state).p_forks == NULL)
	{
		printf("malloc of p_forks failed.\n");
		return (-1);
	}
	i = 0;
	while (i < state->number_of_philosophers)
	{
		(*state).p_philosophers[i].id = i + 1;
		(*state).p_forks[i].id = i + 1;
		i++;
	}
	return (0);
}

int	init(t_state *state, int argc, char **argv)
{
	if (init_arguments(state, argc, argv) == -1 || init_structs(state) == -1)
		return (-1);
}

int main(int argc, char **argv)
{
	t_state state;

	if (argc < 5)
	{
		printf("Not enought arguments!\n");
		return (0);
	}
	if (init(&state, argc, argv) == -1)
		return (-1);
	
printf("number_of_philosophers:%i \ntime_to_die:%i \ntime_to_eat:%i \ntime_to_sleep:%i \nnumber_of_times_each_philosopher_must_eat:%i\n", state.number_of_philosophers, state.time_to_die, state.time_to_eat, state.time_to_sleep, state.number_of_times_each_philosopher_must_eat);
	
	int i = 0;
	while (i < state.number_of_philosophers)
	{
		printf("phili_id:%i fork_id:%i\n", (state).p_philosophers[i].id, (state).p_forks[i].id);
		i++;
	}

	
	
	return (0);
}