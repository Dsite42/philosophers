/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/21 12:15:24 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <time.h>
#include <sys/time.h>

int	check_number_of_arguments(int argc)
{
	if (argc < 5 || argc > 6)
	{
		if (argc < 5)
			printf("Not enought arguments!\n");
		else
			printf("Too many arguments!\n");
		return (-1);
	}
	return (0);
}

void	deinit_structs(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (pthread_mutex_trylock(&state->p_philos[i].mutex) == 0)
		{
			pthread_mutex_unlock(&state->p_philos[i].mutex);
			pthread_mutex_destroy(&state->p_philos[i].mutex);
		}
		if (pthread_mutex_trylock(&state->p_forks[i].mutex) == 0)
		{
			pthread_mutex_unlock(&state->p_forks[i].mutex);
			pthread_mutex_destroy(&state->p_forks[i].mutex);
		}
		i++;
	}
	free(state->p_philos);
	free(state->p_forks);
	free(state->p_print_mutex);
}

static void	detach_threads(t_state *state, pthread_t *philo_threads)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		pthread_detach(philo_threads[i]);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_state		*state;
	pthread_t	*philo_threads;

	if (check_number_of_arguments(argc) == -1)
		return (-1);
	state = (t_state *) malloc(ft_atoi(argv[2]) * sizeof(t_state));
	if (state == NULL)
		return (printf("malloc of state failed.\n"), -1);
	if (init(state, argc, argv) == -1)
		return (-1);
	if (init_threads(state, &philo_threads) == -1)
		return (-1);
	if (create_threads(state, philo_threads) == -1)
		return (-1);
	while (is_program_end(state) == 0)
	{
		ft_wait(9000);
	}
	deinit_structs(state);
	//if (wait_for_threads(state, philo_threads) == -1)
	//	return (-1);
	detach_threads(state, philo_threads);
	free(state);
	free(philo_threads);
	return (0);
}