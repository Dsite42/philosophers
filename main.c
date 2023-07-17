/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/17 11:54:47 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <time.h>
#include <sys/time.h>

int	check_number_of_arguments(int argc)
{
	if (argc < 5 || argc > 6)
	{
		if (argc < 5)
			printf("Not enought arguments!\n");
		else
			printf("Too many arguments!\n");
		return (-1);
	}
	return (0);
}

int	is_death(t_state *state)
{
	int				i;
	struct timeval	tv;
	int				all_must_eat_reached;
	long long time_stamp;

	all_must_eat_reached = 1;
	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	i = 0;
	while (i < state->number_of_philosophers)
	{
		pthread_mutex_lock(&state[i].p_philosophers[i].mutex);
		gettimeofday(&tv, NULL);
		//printf("scheißdiff:%i\n", ((long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec) - state[i].p_philosophers[i].last_meal > (long long)state->time_to_die * (long long)1000);
		//printf("fuck diff:%llu philo_id:%i test:%ld %i\n", (((long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec) - state[i].p_philosophers[i].last_meal) / 1000LL, i, tv.tv_sec, tv.tv_usec);
		if (((long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec) - state[i].p_philosophers[i].last_meal > (long long)state->time_to_die * (long long)1000)
		{
			pthread_mutex_lock(state->p_print_mutex);
			time_stamp = (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec - state->start_time;
			printf("%lli %i died last_meal:%llu diff:%llu\n", time_stamp / 1000, state[i].p_philosophers[i].id, (state[i].p_philosophers[i].last_meal - state->start_time) / 1000, (long long)((long long)((long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec) - state[i].p_philosophers[i].last_meal) / (long long)1000);
			state[i].p_philosophers[i].death_flag = 1;
			printf("philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i", state->p_philosophers[0].id, state->p_philosophers[0].eat_counter, state->p_philosophers[1].id, state->p_philosophers[1].eat_counter, state->p_philosophers[2].id, state->p_philosophers[2].eat_counter, state->p_philosophers[3].id, state->p_philosophers[3].eat_counter, state->p_philosophers[4].id, state->p_philosophers[4].eat_counter);
			pthread_mutex_unlock(&state[i].p_philosophers[i].mutex);
			//exit(0);
			return (1);
		}
		if (state->number_of_times_each_philosopher_must_eat != 0 && state[i].p_philosophers[i].eat_counter < state->number_of_times_each_philosopher_must_eat)
			all_must_eat_reached = 0;
		pthread_mutex_unlock(&state[i].p_philosophers[i].mutex);
		i++;
	}
	if (state->number_of_times_each_philosopher_must_eat != 0 && all_must_eat_reached == 1)
	{
		pthread_mutex_lock(state->p_print_mutex);
		printf("All philosophers ate at least %i times. philo1:%i philo2:%i philo3:%i philo4:%i philo5:%i\n", state->number_of_times_each_philosopher_must_eat, state->p_philosophers[0].eat_counter, state->p_philosophers[1].eat_counter, state->p_philosophers[2].eat_counter, state->p_philosophers[3].eat_counter, state->p_philosophers[4].eat_counter);
		return (1);
	}
	return (0);
}

void	deinit_structs(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		pthread_mutex_destroy(&state->p_philosophers[i].mutex);
		pthread_mutex_destroy(&state->p_forks[i].mutex);
		i++;
	}
	pthread_mutex_destroy(state->p_print_mutex);
	free(state->p_philosophers);
	free(state->p_forks);
	free(state->p_print_mutex);
	free(state);
}

int	main(int argc, char **argv)
{
	t_state		*state;
	pthread_t	*philo_threads;


	if (check_number_of_arguments(argc) == -1)
		return (-1);
	state = (t_state *) malloc(ft_atoi(argv[2]) * sizeof(t_state));
	if (state == NULL)
	{
		printf("malloc of state failed.\n");
		return (-1);
	}
	if (init(state, argc, argv) == -1)
		return (-1);
	/*
	printf("number_of_philosophers:%i \ntime_to_die:%i \ntime_to_eat:%i \ntime_to_sleep:%i \nnumber_of_times_each_philosopher_must_eat:%i\n", state->number_of_philosophers, state->time_to_die, state->time_to_eat, state->time_to_sleep, state->number_of_times_each_philosopher_must_eat);
	int i = 0;
	while (i < state->number_of_philosophers)
	{
		printf("phili_id:%i time:%llu\n", state[i].p_philosophers[i].id, state[i].p_philosophers[i].last_meal);
		i++;
	}
	*/
	if (init_threads(state, &philo_threads) == -1)
		return (-1);
	if (create_threads(state, philo_threads) == -1)
		return (-1);
	while (is_death(state) == 0)
	{
		usleep(9000);
	}
	//exit(0);
	deinit_structs(state);
	free(philo_threads);
	return (0);
	if (wait_for_threads(state, philo_threads) == -1)
		return (-1);
	return (0);
}
