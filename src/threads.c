/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:30:01 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/22 14:45:23 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>

int	wait_for_threads(t_state *state, pthread_t *philo_threads)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (pthread_join(philo_threads[i], NULL) != 0)
		{
			printf("pthread_join failed.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	create_threads(t_state *state, pthread_t *philo_threads)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (pthread_create(&philo_threads[i], NULL,
				(void *(*)(void *))philo_thread, (void *) &state[i]) != 0)
		{
			printf("pthread_create failed.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_threads(t_state *state, pthread_t	**philo_threads)
{
	*philo_threads = (pthread_t *) malloc(state->number_of_philosophers
			* sizeof(pthread_t));
	if (*philo_threads == NULL)
	{
		printf("malloc of philo_threads failed.\n");
		return (-1);
	}
	return (0);
}
