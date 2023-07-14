/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:26 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/14 09:34:14 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_philosopher
{
	int				id;
	int				eat_counter;
	long long		last_meal;
	int				death_flag;
	pthread_mutex_t	mutex;

}	t_philosopher;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;

}	t_fork;

typedef struct s_state
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	t_philosopher	*p_philosophers;
	t_fork			*p_forks;
	pthread_mutex_t	*p_print_mutex;
	int				current_philo_id;
}	t_state;

// init.c
int		init(t_state *state, int argc, char **argv);

// utils.c
int		ft_atoi(const char *nprt);

// philo.c
void	*philo_thread(void *arg);

// threads.c
int		init_threads(t_state *state, pthread_t	**philo_threads);
int		create_threads(t_state *state, pthread_t *philo_threads);
int		wait_for_threads(t_state *state, pthread_t *philo_threads);


#endif