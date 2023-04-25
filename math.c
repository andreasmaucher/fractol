/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 11:12:15 by amaucher          #+#    #+#             */
/*   Updated: 2023/04/25 11:12:18 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_cpx	ft_add_cpx(t_cpx a, t_cpx b)
{
	t_cpx	added;

	added.x = a.x + b.x;
	added.y = a.y + b.y;
	return (added);
}

t_cpx	ft_sub_cpx(t_cpx a, t_cpx b)
{
	t_cpx	subtract;

	subtract.x = a.x - b.x;
	subtract.y = a.y - b.y;
	return (subtract);
}

t_cpx	ft_multi_cpx(t_cpx a, t_cpx b)
{
	t_cpx	multi;

	multi.x = a.x * b.x;
	multi.y = a.y * b.y;
	return (multi);
}

