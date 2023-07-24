/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:04:27 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/24 14:54:25 by cgodecke         ###   ########.fr       */
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
		return (printf("malloc of p_philos failed.\n"), -1);
	(*state).p_forks = (t_fork *)
		malloc(state->number_of_philosophers * sizeof(t_fork));
	if ((*state).p_forks == NULL)
		return (printf("malloc of p_forks failed.\n"), -1);
	(*state).p_dead = (t_dead *)malloc(sizeof(t_dead));
	if ((*state).p_dead == NULL)
		return (printf("malloc of p_dead failed.\n"), -1);
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

static int	init_dead_mutex(t_state *state)
{
	if (pthread_mutex_init(&state->p_dead->mutex, NULL) != 0)
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
	if (init_fork_mutexes(state) == -1)
		return (-1);
	if (init_philo_mutexes(state) == -1)
		return (-1);
	if (init_dead_mutex(state) == -1)
		return (-1);
	return (1);
}
