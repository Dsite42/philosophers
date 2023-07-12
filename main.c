/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/12 11:19:58 by cgodecke         ###   ########.fr       */
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
	struct timeval	*restrict tv;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	if (gettimeofday(tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}

	printf("%i %i %s\n", tv->tv_usec, state->current_philo_id, message); 
	return (0);
}

int	is_death(t_state *state)
{
	int i;
	struct timeval	*restrict tv;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	if (tv == NULL)
	{
		printf("malloc of tv failed.\n");
		return (-1);
	}
	if (gettimeofday(tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (tv->tv_usec - state->p_philosophers[i].last_meal > state->time_to_die && state->p_philosophers[i].eat_counter != 0)
		{
			if (state->p_philosophers[i].death_flag == 0)
			{
				printf("%i %i died Currend philoid:%i last_meal:%li diff:%li\n", tv->tv_usec, i, state->current_philo_id, state->p_philosophers[i].last_meal, tv->tv_usec - state->p_philosophers[i].last_meal);
				state->p_philosophers[i].death_flag = 1;
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	*philo_thread(void *arg)
{
	t_state	*state;
	struct timeval	*restrict tv;

	state = (t_state *)arg;	
	tv = (struct timeval *)malloc(sizeof(struct timeval));
	if (tv == NULL)
	{
		printf("malloc of tv failed.\n");
		//return (-1);
	}

	while (1)
	{
		if(is_death(state) == 1)
			pthread_exit(NULL);
		// Thinking
		print_state_change("is thinking", state);
		//usleep(state->time_to_think);
		//Acquire forks and start eating
		if (state->current_philo_id == 0)
		{
			// Acquire right fork
			pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
			print_state_change("has taken a fork", state);
			 //Acquire left fork
			pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
			print_state_change("has taken a fork", state);
		}
		else
		{
			 //Acquire left fork
			pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
			print_state_change("has taken a fork", state);
			// Acquire right fork
			pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
			print_state_change("has taken a fork", state);
	
		}
		// Eating
		print_state_change("is eating", state);
		usleep(state->time_to_eat);
		if (gettimeofday(tv, NULL) == -1)
		{
			printf("gettimeofday failed.\n");
			//return (-1);
		}
		(*state).p_philosophers[state->current_philo_id].last_meal = tv->tv_usec;
		(*state).p_philosophers[state->current_philo_id].eat_counter++;
	// Release forks after eating
		// Release right fork
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
		// Release left fork
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
	// Sleeping
		print_state_change("is sleeping", state);

		usleep(state->time_to_sleep);
		printf("philo_id:%i eat_counter:%i\n", state->current_philo_id, state->p_philosophers[state->current_philo_id].eat_counter);
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
		//(*state).current_philo_id = i;
		if (pthread_create(&philo_threads[i], NULL,
				(void *(*)(void *))philo_thread, (void *) &state[i]) != 0)
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
	t_state		*state;
	pthread_t	*philo_threads;


	if (check_number_of_arguments(argc) == -1)
		return (-1);
	state = (t_state *) malloc(ft_atoi(argv[2]) * sizeof(t_state));
	if (state == NULL)
	{
		printf("malloc of state failed.\n");
		return (-1);
	}
	if (init(state, argc, argv) == -1)
		return (-1);

printf("number_of_philosophers:%i \ntime_to_die:%i \ntime_to_eat:%i \ntime_to_sleep:%i \nnumber_of_times_each_philosopher_must_eat:%i\n", state->number_of_philosophers, state->time_to_die, state->time_to_eat, state->time_to_sleep, state->number_of_times_each_philosopher_must_eat);

	int i = 0;
	while (i < state->number_of_philosophers)
	{
		printf("phili_id:%i fork_id:%i\n", state->p_philosophers[i].id, (state)->p_forks[i].id);
		i++;
	}
	printf("philoIDDD:%i\n", state[1].current_philo_id);
	if (init_threads(state, &philo_threads) == -1)
		return (-1);
	if (create_threads(state, philo_threads) == -1)
		return (-1);
	if (wait_for_threads(state, philo_threads) == -1)
		return (-1);

	return (0);
}