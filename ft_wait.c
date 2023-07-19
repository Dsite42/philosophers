/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wait.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 16:38:29 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/19 15:01:39 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
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
