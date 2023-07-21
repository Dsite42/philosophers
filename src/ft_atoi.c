/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgodecke <cgodecke@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 17:34:40 by cgodecke          #+#    #+#             */
/*   Updated: 2023/07/12 11:24:31 by cgodecke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This function ft_atoi converts the string to an int.The string may begin with
// an arbitrary amount of white space (as determined by isspace(3)) followed
// by a single optional '+' or '-' sign.
// Return: The converted string number as int.

#include "philo.h"

static int	is_spmn(const char *str, int *minus)
{
	int		i;

	i = 0;
	while ((str[i] == ' ' || str[i] == '\f' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\t' || str[i] == '\v'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*minus = 1;
		i++;
	}
	return (i);
}

int	ft_atoi(const char *nprt)
{
	int	start_n;
	int	minus;
	int	int_res;
	int	i;

	minus = 0;
	start_n = is_spmn (nprt, &minus);
	i = start_n;
	int_res = 0;
	while (nprt[i] >= '0' && nprt[i] <= '9' && nprt[i] != '\0')
		i++;
	i = 0;
	while (nprt[start_n] >= '0' && nprt[start_n] <= '9'
		&& nprt[start_n] != '\0')
	{
		int_res = (int_res * 10) + (nprt[start_n] - 48);
		i++;
		start_n++;
	}
	if (minus == 1)
		int_res = int_res * -1;
	return (int_res);
}
