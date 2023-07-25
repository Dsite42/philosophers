/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:10:57 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/25 16:46:08 by cgodecke         ###   ########.fr       */
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

static int	am_i_dead(t_state *state, struct timeval *tv)
{
	if (((long long)tv->tv_sec * 1000000LL + (long long)tv->tv_usec)
		- state->p_philos[state->current_philo_id].last_meal
		> (long long)state->time_to_die * (long long)1000)
		return (1);
	return (0);
}

static int	eating(t_state *state)
{
	struct timeval	tv;

	pthread_mutex_lock(&state->p_philos[state->current_philo_id].mutex);
	gettimeofday(&tv, NULL);
	if (am_i_dead(state, &tv) == 1)
	{
		pthread_mutex_lock(&state->p_dead->mutex);
		state->p_dead->dead = 1;
		pthread_mutex_unlock(&state->p_dead->mutex);
		release_forks(state);
		pthread_mutex_unlock(&state->p_philos[state->current_philo_id].mutex);
		return (0);
	}
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
	return (1);
}

void	*philo_thread(void *arg)
{
	t_state			*state;

	state = (t_state *)arg;
	if (state->current_philo_id % 2 == 1)
		ft_wait(state->time_to_eat / 2 * 1000);
	while (1)
	{
		if (is_dead_flag(state) == 0)
			print_state_change("is thinking", state);
		else
			pthread_exit(NULL);
		if (state->number_of_philosophers == 1)
			pthread_exit(NULL);
		acquire_forks(state);
		if (eating(state) == 1)
			release_forks(state);
		if (is_dead_flag(state) == 0)
			print_state_change("is sleeping", state);
		else
			pthread_exit(NULL);
		ft_wait(state->time_to_sleep * 1000);
		if (state->number_of_philosophers % 2 == 1 && state->current_philo_id % 2 == 0)
			usleep(state->time_to_eat / 2 * 1000);
			//ft_wait(state->time_to_eat / 2 * 1000);
	}
	pthread_exit(NULL);
}
