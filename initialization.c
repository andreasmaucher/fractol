/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 11:01:45 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/11 11:01:47 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_cpx	*initialization_cpx(double real, double imag)
{
	t_cpx	*cpx;

	cpx = malloc (sizeof(t_cpx));
	if (cpx == NULL)
		return (NULL);
	cpx->real = real;
	cpx->imag = imag;
	return (cpx);
}

t_point	*initialize_point(double real, double imag)
{
	t_point		*point;

	point = malloc (sizeof(t_point));
	if (point == NULL)
		point = NULL;
	point->pos = initialization_cpx(real, imag);
	return (point);
}

t_cursor	*initialize_cursor(void)
{
	t_cursor		*cursor;

	cursor = malloc (sizeof(t_cursor));
	if (cursor == NULL)
		cursor = NULL;
	cursor->before_zoom = initialize_point(0, 0);
	cursor->after_zoom = initialize_point(0, 0);
	return (cursor);
}

void	set_specific_initialization(t_fractol *fractol, char *set_name,
		char *x, char *y)
{
	if (ft_strncmp(set_name, "Mandelbrot", 10) == 0 
		|| ft_strncmp(set_name, "mandelbrot", 10) == 0)
	{
		fractol->mandelbrot = 1;
		(void)x;
		(void)y;
		fractol->julia_r = 0;
		fractol->julia_i = 0;
	}
	else
	{
		fractol->mandelbrot = 0;
		fractol->julia_r = string_to_float(x);
		fractol->julia_i = string_to_float(y);
	}
}

int	initialize_mlx(t_fractol *fractol)
{
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	if (!(fractol->window))
	{
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	if (!(fractol->image))
	{
		mlx_close_window(fractol->window);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (mlx_image_to_window(fractol->window, fractol->image, 0, 0) == -1)
	{
		mlx_close_window(fractol->window);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	return (0);
}
