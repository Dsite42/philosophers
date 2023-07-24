/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 16:54:26 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/24 17:03:36 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>

void	fill_further_states(t_state *state)
{
	int	i;

	i = 1;
	while (i < state->number_of_philosophers)
	{
		state[i].number_of_philosophers = state->number_of_philosophers;
		state[i].time_to_die = state->time_to_die;
		state[i].time_to_eat = state->time_to_eat;
		state[i].time_to_sleep = state->time_to_sleep;
		state[i].number_of_times_each_philosopher_must_eat
			= state->number_of_times_each_philosopher_must_eat;
		state[i].p_philos = state->p_philos;
		state[i].p_forks = state->p_forks;
		state[i].p_dead = state->p_dead;
		state[i].current_philo_id = i;
		state[i].start_time = state->start_time;
		i++;
	}
}

void	fill_state(t_state *state)
{
	int				i;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	i = 0;
	while (i < state->number_of_philosophers)
	{
		(*state).p_philos[i].id = i + 1;
		(*state).p_philos[i].eat_counter = 0;
		(*state).p_forks[i].id = i + 1;
		(*state).current_philo_id = 0;
		(*state).p_dead->dead = 0;
		(*state).start_time = (long long)tv.tv_sec
			* (long long)1000000 + (long long)tv.tv_usec;
		(*state).p_philos[i].last_meal = state->start_time;
		i++;
	}
	fill_further_states(state);
}
