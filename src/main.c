/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/26 16:46:16 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int	is_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < 48 || str[i] > 57)
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_arguments(int argc, char **argv)
{
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) <= 0
		|| ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0
		|| is_digits(argv[1]) == 0 || is_digits(argv[2]) == 0
		|| is_digits(argv[3]) == 0 || is_digits(argv[4]) == 0)
	{
		printf("No valid input. Please insert positive integers.\n");
		return (-1);
	}
	if (argc == 6 && (ft_atoi(argv[5]) <= 0 || is_digits(argv[5]) == 0))
	{
		printf("No valid input. Please insert\
 positive integers higher then 0.\n");
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
		pthread_mutex_destroy(&state->p_philos[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&state->p_dead->mutex);
	free(state->p_philos);
	free(state->p_forks);
	free(state->p_dead);
}

int	main(int argc, char **argv)
{
	t_state		*state;
	pthread_t	*philo_threads;

	if (check_number_of_arguments(argc) == -1 
		|| is_valid_arguments(argc, argv) == -1)
		return (-1);
	state = (t_state *) malloc(ft_atoi(argv[1]) * sizeof(t_state));
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
	wait_for_threads(state, philo_threads);
	deinit_structs(state);
	free(state);
	free(philo_threads);
	return (0);
}
