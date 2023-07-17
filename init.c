/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:04:27 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/17 11:30:36 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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

static int	init_arguments(t_state *state, int argc, char **argv)
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

void	fill_state(t_state *state)
{	
	int	i;
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		//return (-1);
	}

	i = 0;
	while (i < state->number_of_philosophers)
	{
		(*state).p_philosophers[i].id = i + 1;
		(*state).p_philosophers[i].eat_counter = 0;
		(*state).p_forks[i].id = i + 1;
		(*state).current_philo_id = 0;
		(*state).start_time = (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec;
		(*state).p_philosophers[i].last_meal = state->start_time;
		
		i++;
	}
	i = 1;
	while (i < state->number_of_philosophers)
	{
		state[i].number_of_philosophers = state->number_of_philosophers;
		state[i].time_to_die = state->time_to_die;
		state[i].time_to_eat = state->time_to_eat;
		state[i].time_to_sleep = state->time_to_sleep;
		state[i].number_of_times_each_philosopher_must_eat
			= state->number_of_times_each_philosopher_must_eat;
		state[i].p_philosophers = state->p_philosophers;
		state[i].p_forks = state->p_forks;
		state[i].p_print_mutex = state->p_print_mutex;
		state[i].current_philo_id = i;
		state[i].start_time = state->start_time;
		i++;
	}
}

static int	init_structs(t_state *state)
{
	(*state).p_philosophers = (t_philosopher *)
		malloc((state->number_of_philosophers) * sizeof(t_philosopher));
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
		if (pthread_mutex_init(&state->p_philosophers[i].mutex, NULL) != 0)
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
	if (init_fork_mutexes(state) == -1)
		return (-1);
	if (init_philo_mutexes(state) == -1)
		return (-1);
	if (init_print_mutex(state) == -1)
		return (-1);
	return (1);
}
