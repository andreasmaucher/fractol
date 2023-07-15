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

static	t_fractol	*setup_hooks(t_fractol *fractol)
{
	mlx_scroll_hook(fractol->window, &zoom_hook, fractol); //!ZOOM
	mlx_loop_hook(fractol->window, ft_escape_key, fractol);
	//mlx_resize_hook(fractol->window, ft_resize, fractol);
	mlx_loop(fractol->window); //keeps the window open until its closed by the user
	mlx_terminate(fractol->window);
	return(fractol);
}

//! error check mlx functions
static	t_fractol	*initialize_fractol(t_fractol *fractol)//double x, double y
{
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	mlx_image_to_window(fractol->window, fractol->image, 0, 0);
	fractol->cursor = initialize_cursor();
	fractol->value = 1;
	fractol->shift = 0;
	fractol->type = 2;
	color_fractol(fractol);
	setup_hooks(fractol);
	return (fractol);
}

//typedef void (*mlx_resizefunc)(int32_t width, int32_t height, void* param);
//void mlx_resize_hook(mlx_t* mlx, mlx_resizefunc func, void* param)


int main(int ac, char **av)
{
	t_fractol	*fractol;

	fractol = malloc (sizeof(t_fractol));
		if (fractol == 0)
			return (0);
	if (ac == 2)
	{
		if (strncmp(av[1], "Mandelbrot", 10) == 0 || strncmp(av[1], "mandelbrot", 10) == 0)
		{
			fractol->mandelbrot = 1;
			fractol = initialize_fractol(fractol);
			if (fractol == NULL)
				return (EXIT_FAILURE);
		}
	}
	else if (ac == 4) //!need my own strncmp!!!
	{
		if (strncmp(av[1], "Julia", 5) == 0 || strncmp(av[1], "julia", 5) == 0)
		{
			printf("CHECK1");
			printf("%d", check_if_num(av[2]) == 0);
			if (check_if_num(av[2]) && check_if_num(av[3]))
			{
				printf("CHECK2");
				fractol->julia_r = string_to_float(av[2]);
				fractol->julia_i = string_to_float(av[3]);
				fractol = initialize_fractol(fractol);
			}
			else
				return (EXIT_FAILURE);
			/* if (fractol == NULL)
				return (EXIT_FAILURE); */
		}
	}
	/* else
		{
			input_instructions();
			return (EXIT_FAILURE);
		} */
	free(fractol);
	return (EXIT_SUCCESS);
}