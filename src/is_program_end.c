/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_program_end.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 14:10:58 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/25 17:27:57 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int	is_must_eat_reached(t_state *state, int *must_eat_reached)
{
	if (state->number_of_times_each_philosopher_must_eat != 0
		&& *must_eat_reached == 1)
	{
		pthread_mutex_lock(&state->p_dead->mutex);
		state->p_dead->dead = 1;
		pthread_mutex_unlock(&state->p_dead->mutex);
		return (1);
	}
	return (0);
}

static void	set_must_eat_reached(t_state *state, int *must_eat_reached, int i)
{
	if (state->number_of_times_each_philosopher_must_eat != 0 
		&& state[i].p_philos[i].eat_counter
		< state->number_of_times_each_philosopher_must_eat)
		*must_eat_reached = 0;
}

static int	is_dead(t_state *state, struct timeval *tv,
	int *must_eat_reached, int i)
{
	long long	time_stamp;

	while (i < state->number_of_philosophers)
	{
		pthread_mutex_lock(&state[i].p_philos[i].mutex);
		gettimeofday(tv, NULL);
		//printf("philo_id:%i last_meal:%llu diff:%llu\n", state[i].p_philos[i].id, (state[i].p_philos[i].last_meal - state->start_time) / 1000, (long long)((long long)((long long)tv->tv_sec * (long long)1000000 + (long long)tv->tv_usec) - state[i].p_philos[i].last_meal) / (long long)1000);
		if (((long long)tv->tv_sec * 1000000LL + (long long)tv->tv_usec)
			- state[i].p_philos[i].last_meal
			> (long long)state->time_to_die * (long long)1000)
		{
			pthread_mutex_lock(&state->p_dead->mutex);
			state->p_dead->dead = 1;
			time_stamp = (long long)tv->tv_sec * (long long)1000000
				+ (long long)tv->tv_usec - state->start_time;
			printf("%lli %i died\n", time_stamp / 1000, state[i].p_philos[i].id);
//			printf("%lli %i died last_meal:%llu diff:%llu\n", time_stamp / 1000, state[i].p_philos[i].id, (state[i].p_philos[i].last_meal - state->start_time) / 1000, (long long)((long long)((long long)tv->tv_sec * (long long)1000000 + (long long)tv->tv_usec) - state[i].p_philos[i].last_meal) / (long long)1000);
//printf("philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i\n", state->p_philos[0].id, state->p_philos[0].eat_counter, state->p_philos[1].id, state->p_philos[1].eat_counter, state->p_philos[2].id, state->p_philos[2].eat_counter, state->p_philos[3].id, state->p_philos[3].eat_counter, state->p_philos[4].id, state->p_philos[4].eat_counter);
			pthread_mutex_unlock(&state->p_dead->mutex);
			pthread_mutex_unlock(&state[i].p_philos[i].mutex);
			return (1);
		}
		set_must_eat_reached(state, must_eat_reached, i);
		pthread_mutex_unlock(&state[i].p_philos[i].mutex);
		i++;
	}
	return (0);
}

int	is_program_end(t_state *state)
{
	int				i;
	struct timeval	tv;
	int				must_eat_reached;

	must_eat_reached = 1;
	i = 0;
	if (is_dead(state, &tv, &must_eat_reached, i) == 1)
		return (1);
	if (is_must_eat_reached(state, &must_eat_reached) == 1)
		return (1);
	return (0);
}
