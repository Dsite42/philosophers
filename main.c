/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/08 15:10:53 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
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

int	print_state_change(char *message, t_state *state)
{
	struct timeval *restrict tv;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	if (gettimeofday(tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}


	printf("%li %i %s\n", tv->tv_usec, state->current_philo_id, message); 
}

void	*philo_thread(void *arg)
{
	t_state	*state;
	
	state = (t_state *)arg;	
	while (1)
	{
		// Thinking
		print_state_change("is thinking", state);

		//usleep(state->time_to_think);

		// Acquire forks and start eating
		print_state_change("has taken a fork", state);
		// Acquire left fork
		pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
		print_state_change("has taken a fork", state);
		// Acquire right fork
		pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);

		// Eating
		print_state_change("is eating", state);

		usleep(state->time_to_eat);

		// Release forks after eating
		// Release right fork
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
		// Release left fork
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);

		// Sleeping
		print_state_change("is sleeping", state);

		usleep(state->time_to_sleep);
	}
	pthread_exit(NULL);
}

int	wait_for_threads(t_state *state, pthread_t *philo_threads)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (pthread_join(philo_threads[i], NULL) != 0)
		{
			printf("pthread_join failed.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	create_threads(t_state *state, pthread_t *philo_threads)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		(*state).current_philo_id = i;
		if (pthread_create(&philo_threads[i], NULL,
				(void *(*)(void *))philo_thread, (void *) state) != 0)
		{
			printf("pthread_create failed.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_threads(t_state *state, pthread_t	**philo_threads)
{
	*philo_threads = (pthread_t *) malloc(state->number_of_philosophers
			* sizeof(pthread_t));
	if (*philo_threads == NULL)
	{
		printf("malloc of philo_threads failed.\n");
		return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_state		state;
	pthread_t	*philo_threads;


	if (check_number_of_arguments(argc) == -1)
		return (-1);
	if (init(&state, argc, argv) == -1)
		return (-1);

printf("number_of_philosophers:%i \ntime_to_die:%i \ntime_to_eat:%i \ntime_to_sleep:%i \nnumber_of_times_each_philosopher_must_eat:%i\n", state.number_of_philosophers, state.time_to_die, state.time_to_eat, state.time_to_sleep, state.number_of_times_each_philosopher_must_eat);

	int i = 0;
	while (i < state.number_of_philosophers)
	{
		printf("phili_id:%i fork_id:%i\n", (state).p_philosophers[i].id, (state).p_forks[i].id);
		i++;
	}

	if (init_threads(&state, &philo_threads) == -1)
		return (-1);
	if (create_threads(&state, philo_threads) == -1)
		return (-1);
	if (wait_for_threads(&state, philo_threads) == -1)
		return (-1);

	return (0);
}