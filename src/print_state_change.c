/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_state_change.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 18:24:24 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/24 14:53:40 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

int	am_i_dead(t_state *state)
{
	struct timeval	tv;

	pthread_mutex_lock(&state->p_philos[state->current_philo_id].mutex);
	gettimeofday(&tv, NULL);
	if (((long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec)
		- state->p_philos[state->current_philo_id].last_meal
		> (long long)state->time_to_die * (long long)1000)
	{
		pthread_mutex_unlock(&state->p_philos[state->current_philo_id].mutex);
		return (1);
	}
	pthread_mutex_unlock(&state->p_philos[state->current_philo_id].mutex);
	return (0);
}

void	print_first_fork(char *message, t_state *state, int first_fork)
{
	struct timeval	tv;
	long long		time_stamp;

	if (state->p_dead->dead == 1)
	{
		pthread_mutex_unlock(&state->p_forks[first_fork].mutex);
		pthread_mutex_unlock(&state->p_dead->mutex);
		pthread_exit(NULL);
	}
	gettimeofday(&tv, NULL);
	time_stamp = (long long)tv.tv_sec
		* (long long)1000000 + (long long)tv.tv_usec - state->start_time;
	printf("%lli %i %s\n", time_stamp / 1000,
		state->current_philo_id, message);
	pthread_mutex_unlock(&state->p_dead->mutex);

}

void	print_second_fork(char *message, t_state *state, int first_fork, int second_fork)
{
	struct timeval	tv;
	long long		time_stamp;

	if (state->p_dead->dead == 1)
	{
		pthread_mutex_unlock(&state->p_forks[first_fork].mutex);
		pthread_mutex_unlock(&state->p_forks[second_fork].mutex);
		pthread_mutex_unlock(&state->p_dead->mutex);
		pthread_exit(NULL);
	}
	gettimeofday(&tv, NULL);
	time_stamp = (long long)tv.tv_sec
		* (long long)1000000 + (long long)tv.tv_usec - state->start_time;
	printf("%lli %i %s\n", time_stamp / 1000,
		state->current_philo_id, message);
	pthread_mutex_unlock(&state->p_dead->mutex);
}



void	print_state_change(char *message, t_state *state)
{
	struct timeval	tv;
	long long		time_stamp;

	gettimeofday(&tv, NULL);
	time_stamp = (long long)tv.tv_sec
		* (long long)1000000 + (long long)tv.tv_usec - state->start_time;
	printf("%lli %i %s\n", time_stamp / 1000,
		state->current_philo_id + 1, message);
	pthread_mutex_unlock(&state->p_dead->mutex);

}
