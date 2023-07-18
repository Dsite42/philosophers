/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 13:12:52 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/18 13:15:24 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>

int	get_time(struct timeval *tv)
{
	if (gettimeofday(tv, NULL) == -1)
	{
		printf("gettimeofday failed.\n");
		return (-1);
	}
	return (0);
}
