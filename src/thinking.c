/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 11:33:49 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/26 11:35:09 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static void	even_wait(t_state *state)
{
	if (state->number_of_philosophers % 2 == 1
		&& state->current_philo_id % 2 == 0 
		&& state->p_philos[state->current_philo_id].eat_counter > 0)
		usleep(state->time_to_eat / 2 * 1000);
}

static void	odd_wait(t_state *state)
{
	if (state->current_philo_id % 2 == 1 
		&& state->p_philos[state->current_philo_id].eat_counter == 0)
		usleep(state->time_to_eat / 2 * 1000);
}

void	thinking(t_state *state)
{
	print_state_change("is thinking", state);
	odd_wait(state);
	even_wait(state);
}
