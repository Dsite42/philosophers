/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:33 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/13 14:03:45 by cgodecke         ###   ########.fr       */
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

int	print_state_change(char *message, t_state *state)
{
	struct timeval	*restrict tv;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	pthread_mutex_lock(state->p_print_mutex);
	if (gettimeofday(tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}

	printf("%li%i %i %s\n", tv->tv_sec, tv->tv_usec, state->current_philo_id, message);
	pthread_mutex_unlock(state->p_print_mutex);
	return (0);
}

int	is_death(t_state *state)
{
	int				i;
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	i = 0;
	while (i < state->number_of_philosophers)
	{
		pthread_mutex_lock(&state[i].p_philosophers[i].mutex);
		//printf("fuck diff:%llu philo_id:%i test:%ld %i\n", (((long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec) - state[i].p_philosophers[i].last_meal) / 1000LL, i, tv.tv_sec, tv.tv_usec);

		if (((long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec) - state[i].p_philosophers[i].last_meal > (long long)state->time_to_die * 1000LL)
		{
			//if (state->p_philosophers[i].death_flag == 0)
			//{
				pthread_mutex_lock(state->p_print_mutex);
				printf("%ld%i philo_id:%i died last_meal:%llu diff:%llu\n", tv.tv_sec, tv.tv_usec, state[i].p_philosophers[i].id, state[i].p_philosophers[i].last_meal, (long long)((long long)((long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec) - state[i].p_philosophers[i].last_meal) / (long long)1000);
				state[i].p_philosophers[i].death_flag = 1;
				printf("philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i philo%i eaten:%i", state->p_philosophers[0].id, state->p_philosophers[0].eat_counter, state->p_philosophers[1].id, state->p_philosophers[1].eat_counter, state->p_philosophers[2].id, state->p_philosophers[2].eat_counter, state->p_philosophers[3].id, state->p_philosophers[3].eat_counter, state->p_philosophers[4].id, state->p_philosophers[4].eat_counter);
				pthread_mutex_unlock(&state[i].p_philosophers[i].mutex);
				
				exit(0);
			//}
			return (1);
		}
		pthread_mutex_unlock(&state[i].p_philosophers[i].mutex);
		i++;
	}
	return (0);
}

int	is_must_eat_reached(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->number_of_philosophers)
	{
		if (state->p_philosophers[i].eat_counter 
			< state->number_of_times_each_philosopher_must_eat)
			return (0);
		i++;
	}
	printf("All philosophers ate at least %i times.\n", state->number_of_times_each_philosopher_must_eat);
	return (1);
}

void	*philo_thread(void *arg)
{
	t_state	*state;
	struct timeval	tv;

	state = (t_state *)arg;	

	while (1)
	{
		//if(is_death(state) == 1)
		//	pthread_exit(NULL);
		//if (state->number_of_times_each_philosopher_must_eat > 0 && is_must_eat_reached(state) == 1)
		//	pthread_exit(NULL);
		// Thinking
		print_state_change("is thinking", state);
		//usleep(state->time_to_think);
		//Acquire forks and start eating
		if (state->current_philo_id == 0)
		{
			// Acquire right fork
			pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
			print_state_change("has taken a fork", state);
			 //Acquire left fork
			pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
			print_state_change("has taken a fork", state);
		}
		else
		{
			 //Acquire left fork
			pthread_mutex_lock(&state->p_forks[state->current_philo_id].mutex);
			print_state_change("has taken a fork", state);
			// Acquire right fork
			pthread_mutex_lock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
			print_state_change("has taken a fork", state);
	
		}
		// Eating
		print_state_change("is eating", state);
		usleep(state->time_to_eat * 1000);
	// Release forks after eating
		// Release right fork
		pthread_mutex_unlock(&state->p_forks[(state->current_philo_id + 1) % state->number_of_philosophers].mutex);
		// Release left fork
		pthread_mutex_unlock(&state->p_forks[state->current_philo_id].mutex);

		// Update last_meal
		pthread_mutex_lock(&state->p_philosophers[state->current_philo_id].mutex);
		if (gettimeofday(&tv, NULL) == -1)
		{
			printf("gettimeofday failed.\n");
			//return (-1);
		}
		(*state).p_philosophers[state->current_philo_id].last_meal = (unsigned long long)(tv.tv_sec * 1000000LL + tv.tv_usec);
		//printf("philo_id:%i last_meal gesetzt:%llu\n", state->current_philo_id, state->p_philosophers[state->current_philo_id].last_meal);
		(*state).p_philosophers[state->current_philo_id].eat_counter++;
		pthread_mutex_unlock(&state->p_philosophers[state->current_philo_id].mutex);

	// Sleeping
		print_state_change("is sleeping", state);

		usleep(state->time_to_sleep * 1000);
		//printf("philo_id:%i eat_counter:%i\n", state->current_philo_id, state->p_philosophers[state->current_philo_id].eat_counter);
	}
	pthread_exit(NULL);
}

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
		//(*state).current_philo_id = i;
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
printf("number_of_philosophers:%i \ntime_to_die:%i \ntime_to_eat:%i \ntime_to_sleep:%i \nnumber_of_times_each_philosopher_must_eat:%i\n", state->number_of_philosophers, state->time_to_die, state->time_to_eat, state->time_to_sleep, state->number_of_times_each_philosopher_must_eat);

	int i = 0;
	while (i < state->number_of_philosophers)
	{
		printf("phili_id:%i time:%llu\n", state[i].p_philosophers[i].id, state[i].p_philosophers[i].last_meal);
		i++;
	}
	//printf("philoIDDD:%i\n", state[1].current_philo_id);

	if (init_threads(state, &philo_threads) == -1)
		return (-1);
	if (create_threads(state, philo_threads) == -1)
		return (-1);
	while (is_death(state) == 0)
	{
		usleep(9000);
	}
	exit(0);

	if (wait_for_threads(state, philo_threads) == -1)
		return (-1);


	//struct timeval	tv;
	//int i = 0;
	//unsigned long long last_meal = 0;
//
	//while (i < 1000)
	//{
	//	if (gettimeofday(&tv, NULL) == -1)
	//	{
	//		printf("gettimeofday failed.\n");
	//		return (-1);
	//	}
	//	printf("time:%llu\n", (unsigned long long)(tv.tv_sec * 1000000 + tv.tv_usec));
	//	printf("fuck diff:%llu philo_id:%i test:%ld %i\n", (((unsigned long long)(tv.tv_sec * 1000000 + tv.tv_usec)) - last_meal) / 1000, i, tv.tv_sec, tv.tv_usec);
//
	//	usleep(10000);
	//	last_meal = (tv.tv_sec * 1000000 + tv.tv_usec);
	//	i++;
	//}
	return (0);
}