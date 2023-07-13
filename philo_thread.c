/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:10:57 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/13 14:28:36 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	print_state_change(char *message, t_state *state)
{
	struct timeval	*restrict tv;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	pthread_mutex_lock(state->p_print_mutex);
	if (gettimeofday(tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	printf("%li%i %i %s\n", tv->tv_sec, tv->tv_usec,
		state->current_philo_id, message);
	pthread_mutex_unlock(state->p_print_mutex);
	return (0);
}

void	*philo_thread(void *arg)
{
	t_state			*state;
	struct timeval	tv;

	state = (t_state *)arg;
	while (1)
	{
		//if (state->number_of_times_each_philosopher_must_eat > 0 && is_must_eat_reached(state) == 1)
		//	pthread_exit(NULL);

		// Thinking
		print_state_change("is thinking", state);

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
		usleep(state->time_to_eat * 1000);

		// Release forks after eating
		// Release right fork
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
		// Release left fork
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);

		// Update last_meal
		pthread_mutex_lock(&state->p_philosophers[state->current_philo_id].mutex);
		if (gettimeofday(&tv, NULL) == -1)
		{
			printf("gettimeofday failed.\n");
			//return (-1);
		}
		(*state).p_philosophers[state->current_philo_id].last_meal = (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
		//printf("philo_id:%i last_meal gesetzt:%llu\n", state->current_philo_id, state->p_philosophers[state->current_philo_id].last_meal);
		(*state).p_philosophers[state->current_philo_id].eat_counter++;
		pthread_mutex_unlock(&state->p_philosophers[state->current_philo_id].mutex);

		// Sleeping
		print_state_change("is sleeping", state);
		usleep(state->time_to_sleep * 1000);
		//printf("philo_id:%i eat_counter:%i\n", state->current_philo_id, state->p_philosophers[state->current_philo_id].eat_counter);
	}
	pthread_exit(NULL);
}
