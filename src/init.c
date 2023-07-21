/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:04:27 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/21 18:03:10 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

static int	init_structs(t_state *state)
{
	(*state).p_philos = (t_philosopher *)
		malloc((state->number_of_philosophers) * sizeof(t_philosopher));
	if ((*state).p_philos == NULL)
	{
		printf("malloc of p_philos failed.\n");
		return (-1);
	}
	(*state).p_forks = (t_fork *)
		malloc(state->number_of_philosophers * sizeof(t_fork));
	if ((*state).p_forks == NULL)
	{
		printf("malloc of p_forks failed.\n");
		return (-1);
	}
	(*state).p_print_mutex = (pthread_mutex_t *)
		malloc(sizeof(pthread_mutex_t));
	if ((*state).p_print_mutex == NULL)
	{
		printf("malloc of print_mutex failed.\n");
		return (-1);
	}
	fill_state(state);
	return (0);
}

static int	init_fork_mutexes(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (pthread_mutex_init(&state->p_forks[i].mutex, NULL) != 0)
		{
			printf("mutex init failed.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	init_philo_mutexes(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (pthread_mutex_init(&state->p_philos[i].mutex, NULL) != 0)
		{
			printf("mutex init failed.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	init_print_mutex(t_state *state)
{
	if (pthread_mutex_init(state->p_print_mutex, NULL) != 0)
	{
		printf("mutex init failed.\n");
		return (-1);
	}
	return (0);
}

int	init(t_state *state, int argc, char **argv)
{
	if (init_arguments(state, argc, argv) == -1 || init_structs(state) == -1)
		return (-1);
	if (init_print_mutex(state) == -1)
		return (-1);
	if (init_fork_mutexes(state) == -1)
		return (-1);
	if (init_philo_mutexes(state) == -1)
		return (-1);
	return (1);
}
