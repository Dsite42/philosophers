/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 14:10:57 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/17 15:46:20 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_wait(long time_to_sleep)
{
	struct timeval	tv;
	long long		time_current;
	long long		time_begin;

	gettimeofday(&tv, NULL);
	time_begin = (long long)tv.tv_sec 
		* (long long)1000000 + (long long)tv.tv_usec;
	time_current = time_begin;
	while (time_current - time_begin < time_to_sleep)
	{
		usleep(20);
		gettimeofday(&tv, NULL);
		time_current = (long long)tv.tv_sec 
			* (long long)1000000 + (long long)tv.tv_usec;
	}
}

static int	print_state_change(char *message, t_state *state)
{
	struct timeval	tv;
	long long		time_stamp;

	pthread_mutex_lock(state->p_print_mutex);
	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	time_stamp = (long long)tv.tv_sec 
		* (long long)1000000 + (long long)tv.tv_usec - state->start_time;
	printf("%lli %i %s\n", time_stamp / 1000,
		state->current_philo_id, message);
	pthread_mutex_unlock(state->p_print_mutex);
	return (0);
}

void	*philo_thread(void *arg)
{
	t_state			*state;
	struct timeval	tv;

	state = (t_state *)arg;

	gettimeofday(&tv, NULL);
	long long philo_thread_diff = (long long)((long long)((long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec) - state->p_philosophers[state->current_philo_id].last_meal);
	//printf("time:%ld%i last_meal:%lld philo_thread_diff:%lld phili_id:%i\n", tv.tv_sec, tv.tv_usec, state->p_philosophers[state->current_philo_id].last_meal, philo_thread_diff, state->current_philo_id + 1);
	//if (philo_thread_diff < 100)
	//	usleep(philo_thread_diff * 3100);

	//usleep(state->current_philo_id * 30000);
	//usleep(state->current_philo_id * 2);

	while (1)
	{
		//if (state->number_of_times_each_philosopher_must_eat > 0 && is_must_eat_reached(state) == 1)
		//	pthread_exit(NULL);

		// Thinking
		print_state_change("is thinking", state);

		//Acquire forks and start eating
		if (state->current_philo_id == 0)
		{
			// Acquire right fork
			pthread_mutex_lock(&state->p_forks[(state->current_philo_id+2) % state->number_of_philosophers].mutex);
			print_state_change("has taken a fork", state);
			 //Acquire left fork
			pthread_mutex_lock(&state->p_forks[state->current_philo_id+1].mutex);
			print_state_change("has taken a fork", state);
		}
		else
		{
			 //Acquire left fork
			pthread_mutex_lock(&state->p_forks[state->current_philo_id+1].mutex);
			print_state_change("has taken a fork", state);
			// Acquire right fork
			pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 2) % state->number_of_philosophers].mutex);
			print_state_change("has taken a fork", state);
		}
		// Eating
		pthread_mutex_lock(&state->p_philosophers[state->current_philo_id].mutex);
		// Update last_meal
		//if (gettimeofday(&tv, NULL) == -1)
		//{
		//	printf("gettimeofday failed.\n");
		//	//return (-1);
		//}
		//(*state).p_philosophers[state->current_philo_id].last_meal = (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec;
		//printf("philo_id:%i last_meal gesetzt:%llu\n", state->current_philo_id, state->p_philosophers[state->current_philo_id].last_meal);

		gettimeofday(&tv, NULL);
		(*state).p_philosophers[state->current_philo_id].last_meal = (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec;
		(*state).p_philosophers[state->current_philo_id].eat_counter++;
		pthread_mutex_unlock(&state->p_philosophers[state->current_philo_id].mutex);
		print_state_change("is eating", state);
		//usleep(state->time_to_eat * 1000);
		ft_wait(state->time_to_eat * 1000);

		// Release forks after eating
		// Release right fork
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
		// Release left fork
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);

		

		// Sleeping
		print_state_change("is sleeping", state);
		//usleep(state->time_to_sleep * 1000);
		ft_wait(state->time_to_sleep * 1000);
		//printf("philo_id:%i eat_counter:%i\n", state->current_philo_id, state->p_philosophers[state->current_philo_id].eat_counter);
	}
	pthread_exit(NULL);
}
