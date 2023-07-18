/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/18 14:14:52 by cgodecke         ###   ########.fr       */
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
		pthread_mutex_destroy(&state->p_philosophers[i].mutex);
		pthread_mutex_destroy(&state->p_forks[i].mutex);
		i++;
	}
	pthread_mutex_destroy(state->p_print_mutex);
	free(state->p_philosophers);
	free(state->p_forks);
	free(state->p_print_mutex);
	free(state);
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
	//exit(0);
	deinit_structs(state);
	free(philo_threads);
	return (0);
	if (wait_for_threads(state, philo_threads) == -1)
		return (-1);
	return (0);
}
