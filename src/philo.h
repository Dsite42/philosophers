/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 13:58:26 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/26 11:34:22 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef struct s_philosopher
{
	int				id;
	int				eat_counter;
	long long		last_meal;
	pthread_mutex_t	mutex;

}	t_philosopher;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;

}	t_fork;

typedef struct s_dead
{
	int				dead;
	pthread_mutex_t	mutex;

}	t_dead;

typedef struct s_state
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	t_philosopher	*p_philos;
	t_fork			*p_forks;
	t_dead			*p_dead;
	int				current_philo_id;
	long long		start_time;
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

// ft_wait.c
void	ft_wait(long time_to_sleep);

// fill_state.c
void	fill_state(t_state *state);

// init_arguments.c
int		init_arguments(t_state *state, int argc, char **argv);

// is_program_end.c
int		is_program_end(t_state *state);

// print_state_change.c
void	print_state_change(char *message, t_state *state);
void	print_state_change(char *message, t_state *state);

// forks.c
int		acquire_forks(t_state *state);
void	release_forks(t_state *state);

// dead.c
int		am_i_dead(t_state *state, struct timeval *tv);
int		is_dead_flag(t_state *state);

// thinking.c
void	thinking(t_state *state);

#endif
