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
	n->real = (-1 + 2 * (x / WIDTH)) * WIDTH / HEIGHT * fractol->value;
	n->imag = (1 - 2 * (y / HEIGHT)) * fractol->value;
	return (n);
}

int	create_fractol(double x, double y, t_fractol *fractol)
{
	t_cpx	*z;
	t_cpx	*c;
	int			iterations;

	if (fractol->mandelbrot == 1)
	{
		c = complex_n_conversion(x, y, fractol);
		c = zoom_fractol(c, fractol);
		z = initialization_cpx(0, 0);
	}
	else
	{
		z = complex_n_conversion(x, y, fractol);
		z = zoom_fractol(z, fractol);
		c = initialization_cpx(fractol->julia_r,
				fractol->julia_i);
	}
	iterations = check_stability(z, c);
	//free(z); //!neded?!?!?
	//free(c);
	return (iterations);
}