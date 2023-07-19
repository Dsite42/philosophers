/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:10:57 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/18 14:21:24 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	print_state_change(char *message, t_state *state)
{
	struct timeval	tv;
	long long		time_stamp;

	pthread_mutex_lock(state->p_print_mutex);
	if (get_time(&tv) == -1)
		return (-1);
	time_stamp = (long long)tv.tv_sec 
		* (long long)1000000 + (long long)tv.tv_usec - state->start_time;
	printf("%lli %i %s\n", time_stamp / 1000,
		state->current_philo_id, message);
	pthread_mutex_unlock(state->p_print_mutex);
	return (0);
}

// To avoid deadlock, the first philosopher has to pick up the right fork first
static void	acquire_forks(t_state *state)
{
	if (state->current_philo_id + 1 == state->number_of_philosophers)
	{
		pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1) 
			% state->number_of_philosophers].mutex);
		print_state_change("has taken a fork", state);
		pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
		print_state_change("has taken a fork", state);
	}
	else
	{
		pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
		print_state_change("has taken a fork", state);
		pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		print_state_change("has taken a fork", state);
	}
}

void	eating(t_state *state)
{
	struct timeval	tv;

	pthread_mutex_lock(&state->p_philosophers[state->current_philo_id].mutex);
	get_time(&tv); // error check
	(*state).p_philosophers[state->current_philo_id].last_meal
		= (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec;
	(*state).p_philosophers[state->current_philo_id].eat_counter++;
	pthread_mutex_unlock(&state->p_philosophers[state->current_philo_id].mutex);
	print_state_change("is eating", state);
	ft_wait(state->time_to_eat * 1000);
}

void	release_forks(t_state *state)
{
	pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
		% state->number_of_philosophers].mutex);
	pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
}

void	*philo_thread(void *arg)
{
	t_state			*state;

	state = (t_state *)arg;
	usleep(state->current_philo_id * 100);
	while (1)
	{
		print_state_change("is thinking", state);
		acquire_forks(state);
		eating(state);
		release_forks(state);
		print_state_change("is sleeping", state);
		ft_wait(state->time_to_sleep * 1000);
	}
	pthread_exit(NULL);
}
