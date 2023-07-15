/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 12:18:44 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/15 12:18:47 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double	calc_fractional_part(char *str)
{
	double fractional_result;
    double multiplier;

	fractional_result = 0;
	multiplier = 0.1;
	str++;
	while (*str >= '0' && *str <= '9')
    {
        fractional_result = fractional_result + (*str - '0') * multiplier;
        multiplier /= 10.0;
        str++;
    }
	return (fractional_result);
}

double string_to_float(char *str)
{
	double result;
    double sign;
	double fractional_result;

	sign = 1;
	result = 0;
    while (*str == ' ' || *str == '\t')
        str++;
    if (*str == '-')
    {
        sign = -1.0;
        str++;
    }
    else if (*str == '+')
        str++;
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10.0 + (*str - '0');
        str++;
    }
	fractional_result = 0;
    if (*str == '.')
        fractional_result = calc_fractional_part(str);
    result = (result + fractional_result) * sign;
    return result;
}

int	ft_isdigit(char *str)
{	
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_char_in_str(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

//first check if dot, if yes split it up and check both strings
//if no, check the whole string for numbers
double	check_if_num(char *str)
{
	char	**split_str;
	char	dot;

	dot = '.';
	if (!is_char_in_str(str, dot))
		return (ft_isdigit(str));
	else
	{
		split_str = ft_split(str, dot);
		if (ft_isdigit(split_str[0]) && ft_isdigit(split_str[1]))
			return(1);
	}
	return (0);
}