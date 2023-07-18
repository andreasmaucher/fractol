/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 11:02:45 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/11 11:02:47 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

int	ft_colouring(t_fractol *fractol, unsigned n, unsigned x, unsigned y)
{
	uint32_t	color;

	if (n < ITERATIONS)
	{
		color = ft_pixel(n * 42, n * 84, n * 21, 84);
		mlx_put_pixel(fractol->image, x, y, color);
		return (0);
	}
	else 
	{
		mlx_put_pixel(fractol->image, x, y, ft_pixel(0, 0, 0, 42));
		return (0);
	}
}

uint32_t	color_set(double x, double y, t_fractol *fractol)
{
	int			iter;
	uint32_t	color;

	iter = create_fractol(x, y, fractol);
	if (iter < ITERATIONS)
		color = ft_pixel(iter * 42, iter * 84, iter * 21, 84);
	else
		color = ft_pixel(0, 0, 0, 58);
	return (color);
}

void	color_fractol(t_fractol *fractol)
{
	int				x;
	int				y;
	uint32_t		color;

	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			color = color_set(x, y, fractol);
			mlx_put_pixel(fractol->image, x, y, color);
			y++;
		}
		x++;
	}
}
