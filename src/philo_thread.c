/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:10:57 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/24 12:18:41 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	is_dead_flag(t_state *state)
{
	pthread_mutex_lock(&state->p_dead->mutex);
	if (state->p_dead->dead == 1)
	{
		pthread_mutex_unlock(&state->p_dead->mutex);
		return (1);
	}
	return (0);
}

static void	aquire_forks_last_philo(t_state *state)
{
	pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1)
		% state->number_of_philosophers].mutex);
	if (is_dead_flag(state) == 0)
		print_state_change("has taken a fork", state);
	else
	{
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		pthread_exit(NULL);
	}
	pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
	if (is_dead_flag(state) == 0)
		print_state_change("has taken a fork", state);
	else
	{
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		pthread_exit(NULL);
	}
}

// To avoid deadlock, the first philosopher has to pick up the right fork first
static void	acquire_forks(t_state *state)
{
	if (state->current_philo_id + 1 == state->number_of_philosophers)
		aquire_forks_last_philo(state);
	else
	{
		pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
		if (is_dead_flag(state) == 0)
			print_state_change("has taken a fork", state);
		else
		{
			pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
			pthread_exit(NULL);
		}
		pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		if (is_dead_flag(state) == 0)
			print_state_change("has taken a fork", state);
		else
		{
			pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
				% state->number_of_philosophers].mutex);
			pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
			pthread_exit(NULL);
		}
	}
}

static void	eating(t_state *state)
{
	struct timeval	tv;

	pthread_mutex_lock(&state->p_philos[state->current_philo_id].mutex);
	gettimeofday(&tv, NULL);
	(*state).p_philos[state->current_philo_id].last_meal
		= (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec;
	(*state).p_philos[state->current_philo_id].eat_counter++;
	pthread_mutex_unlock(&state->p_philos[state->current_philo_id].mutex);
	if (is_dead_flag(state) == 0)
		print_state_change("is eating", state);
	else
	{
		release_forks(state);
		pthread_exit(NULL);
	}
	ft_wait(state->time_to_eat * 1000);
}

void	release_forks(t_state *state)
{
	if (state->current_philo_id + 1 == state->number_of_philosophers)
	{
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
	}
	else
	{
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);	
	}
}


void	*philo_thread(void *arg)
{
	t_state			*state;

	state = (t_state *)arg;
	usleep(state->current_philo_id * 1);
	while (1)
	{
		if (is_dead_flag(state) == 0)
			print_state_change("is thinking", state);
		else
			pthread_exit(NULL);
		if (state->number_of_philosophers == 1)
			pthread_exit(NULL);
		acquire_forks(state);
		eating(state);
		release_forks(state);
		if (is_dead_flag(state) == 0)
			print_state_change("is sleeping", state);
		else
			pthread_exit(NULL);
		ft_wait(state->time_to_sleep * 1000);
	}
	pthread_exit(NULL);
}
