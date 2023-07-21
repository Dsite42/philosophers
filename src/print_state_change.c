/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_state_change.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 18:24:24 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/21 21:17:30 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

static int	am_i_dead(t_state *state)
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

void	print_state_change(char *message, t_state *state)
{
	struct timeval	tv;
	long long		time_stamp;

	if (am_i_dead(state))
		pthread_exit(NULL);
	while (pthread_mutex_trylock(state->p_print_mutex) != 0)
	{
		if (am_i_dead(state))
			pthread_exit(NULL);
		usleep(100);
	}
	gettimeofday(&tv, NULL);
	time_stamp = (long long)tv.tv_sec
		* (long long)1000000 + (long long)tv.tv_usec - state->start_time;
	printf("%lli %i %s\n", time_stamp / 1000,
		state->current_philo_id, message);
	pthread_mutex_unlock(state->p_print_mutex);
}
