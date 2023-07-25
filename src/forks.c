/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:49:09 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/25 11:42:05 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	aquire_forks_last_philo(t_state *state)
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

static void	quire_forks_without_last(t_state *state)
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
		pthread_mutex_unlock(
			&state->p_forks[state->current_philo_id].mutex);
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1)
			% state->number_of_philosophers].mutex);
		pthread_exit(NULL);
	}
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
void	acquire_forks(t_state *state)
{
	if (state->current_philo_id + 1 == state->number_of_philosophers)
		aquire_forks_last_philo(state);
	else
		quire_forks_without_last(state);
}
