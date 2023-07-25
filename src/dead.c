/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 19:51:50 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/25 19:52:12 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	am_i_dead(t_state *state, struct timeval *tv)
{
	if (((long long)tv->tv_sec * 1000000LL + (long long)tv->tv_usec)
		- state->p_philos[state->current_philo_id].last_meal
		> (long long)state->time_to_die * (long long)1000)
		return (1);
	return (0);
}
