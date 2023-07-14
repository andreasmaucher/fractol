/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 11:02:13 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/11 11:02:20 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/* responsible for converting the mouse coordinates (x and y) to 
complex number coordinates based on the window dimensions and the zoom value.
Mapping the x & y coordinates of a pixel to the range [-1, 1] */
t_cpx	*complex_n_conversion(double x, double y, t_fractol *fractol)
{
	t_cpx	*n;

	n = malloc (sizeof(t_cpx));
	if (n == NULL)
		return (NULL);
	n->real = (-1 + 2 * (x / WIDTH)) * WIDTH / HEIGHT * fractol->zoom->value;
	n->imag = (1 - 2 * (y / HEIGHT)) * fractol->zoom->value;
	return (n);
}