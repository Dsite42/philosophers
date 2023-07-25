/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:49:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/25 19:54:03 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	quire_forks_without_last(t_state *state)
{
	pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
	if (is_dead_flag(state) == 0)
		print_state_change("has taken a fork", state);
	else
	{
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);
		return (0);
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
		return (0);
	}
	return (1);
}

static int	aquire_forks_last_philo(t_state *state)
{
	pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1)
		% state->number_of_philosophers].mutex);
	if (is_dead_flag(state) == 0)
		print_state_change("has taken a fork", state);
	else
	{
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		return (0);
	}
	pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
	if (is_dead_flag(state) == 0)
		print_state_change("has taken a fork", state);
	else
	{
		pthread_mutex_unlock(
			&state->p_forks[state->current_philo_id].mutex);
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		return (0);
	}
	return (1);
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

// To avoid deadlock, the first philosopher has to pick up the right fork first
int	acquire_forks(t_state *state)
{
	if (state->current_philo_id % 2 == 0)
		return (quire_forks_without_last(state));
	else
		return (aquire_forks_last_philo(state));
}
