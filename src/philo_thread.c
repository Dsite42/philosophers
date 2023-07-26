/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:10:57 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/26 12:06:07 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	is_dont_start_eating(t_state *state, struct timeval *tv)
{
	if (am_i_dead(state, tv) == 1)
	{
		pthread_mutex_lock(&state->p_dead->mutex);
		state->p_dead->dead = 1;
		pthread_mutex_unlock(&state->p_dead->mutex);
		release_forks(state);
		pthread_mutex_unlock(&state->p_philos[state->current_philo_id].mutex);
		return (1);
	}
	return (0);
}

static int	eating(t_state *state)
{
	struct timeval	tv;

	pthread_mutex_lock(&state->p_philos[state->current_philo_id].mutex);
	gettimeofday(&tv, NULL);
	if (is_dont_start_eating(state, &tv) == 1)
		return (0);
	(*state).p_philos[state->current_philo_id].last_meal
		= (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec;
	if ((*state).p_philos[state->current_philo_id].last_meal - state->start_time
		< state->time_to_eat / 2 * 1000)
		(*state).p_philos[state->current_philo_id].last_meal
			= state->start_time;
	(*state).p_philos[state->current_philo_id].eat_counter++;
	pthread_mutex_unlock(&state->p_philos[state->current_philo_id].mutex);
	if (is_dead_flag(state) == 0)
		print_state_change("is eating", state);
	else
	{
		release_forks(state);
		return (0);
	}
	ft_wait(state->time_to_eat * 1000);
	return (1);
}

void	*philo_thread(void *arg)
{
	t_state			*state;

	state = (t_state *)arg;
	while (1)
	{
		if (is_dead_flag(state) == 0)
			thinking(state);
		else
			return (NULL);
		if (state->number_of_philosophers == 1)
			return (NULL);
		if (acquire_forks(state) == 0)
			return (NULL);
		if (eating(state) == 1)
			release_forks(state);
		else
			return (NULL);
		if (is_dead_flag(state) == 0)
			print_state_change("is sleeping", state);
		else
			return (NULL);
		ft_wait(state->time_to_sleep * 1000);
	}
	return (NULL);
}
