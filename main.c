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

/* MinRe, MaxRe: defining the min & max values for the real & imaginary parts
, MinIm, MaxIm: defining boundaries, MaxIm is calculated based on the screen ratio, to avoid
image distortion when the display window isn't a square
Re_factor & Im_factor: used to map screen coordinates to complex numbers, calculated
by dividing the lenght of the rectangle with the screen width, this is
essentially to calculate the step size between consectutive pixels  */
void mandelbrot_algo(t_fractol *fractol)
{
	double MinRe;
	double MaxRe;
	double MinIm;
	double MaxIm;
	double Re_factor;
	double Im_factor;
	unsigned n;
	unsigned y;
	unsigned x;

	MinRe = -4.2;
	MaxRe = 1.5;
	MinIm = -1.5;
	MaxIm = MinIm + (MaxRe-MinRe) * HEIGHT / WIDTH;
	Re_factor = (MaxRe-MinRe) / (WIDTH - 1);
	Im_factor = (MaxIm - MinIm) / (HEIGHT - 1);
	n = 0;
	y = 0;

	while (y < HEIGHT)
	{
		double c_im = MaxIm - y*Im_factor;
		x = 0;
		while (x < WIDTH)
		{
			double c_re;
			c_re = MinRe + x*Re_factor;
			double Z_re;
			Z_re = c_re;
			double Z_im = c_im;
			n = 0;
			while(n < ITERATIONS)  //after this loop we get value between 0 and max iter
			{
				double Z_re2; //a*a
				Z_re2 = Z_re*Z_re; //a*a
				double Z_im2; //b*b
				Z_im2 = Z_im*Z_im;
				if(Z_re2 + Z_im2 > 4)
					break;
				if (fractol->mandelbrot != 1)
				{
					Z_im = 2*Z_re*Z_im + fractol->julia_i; //c_im
					Z_re = Z_re2 - Z_im2 + fractol->julia_r; //c_re
				}
				else
				{
					Z_im = 2*Z_re*Z_im + c_im;
					Z_re = Z_re2 - Z_im2 + c_re;
				}
				n++;
			}
			ft_colouring(fractol, n, x, y);
			++x;
		}
		++y;
	}
}

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

//! add extra if function for Julia
int	create_set(double x, double y, t_fractol *fractol)
{
	t_cpx	*z;
	t_cpx	*c;
	int			iterations;

	if (fractol->mandelbrot == 1)
	{
		c = complex_n_conversion(x, y, fractol);
		c = move_fractol(c, fractol);
		z = initialization_cpx(0, 0);
	}
	else
	{
		z = complex_n_conversion(x, y, fractol);
		z = move_fractol(z, fractol);
		c = initialization_cpx(fractol->julia_r,
				fractol->julia_i);
	}
	iterations = check_stability(z, c);
	free(z);
	free(c);
	return (iterations);
}

uint32_t	color_set(double x, double y, t_fractol *fractol)
{
	int			iter;
	uint32_t	color;

	iter = create_set(x, y, fractol);
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

static	t_fractol	*initialize_window(t_fractol *fractol)
{
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	mlx_image_to_window(fractol->window, fractol->image, 0, 0);
	fractol->cursor = initialize_cursor();
	fractol->value = 1;
	fractol->shift = 0;
	fractol->type = START;
	return(fractol);
}

static	t_fractol	*initialize_fractol(t_fractol *fractol)//double x, double y
{
	//t_fractol	*fractol;

	/* fractol = malloc (sizeof(t_fractol));
	if (fractol == 0)
		return (0); */
	initialize_window(fractol);
	mandelbrot_algo(fractol);
	setup_hooks(fractol);
	return (fractol);
}

//typedef void (*mlx_resizefunc)(int32_t width, int32_t height, void* param);
//void mlx_resize_hook(mlx_t* mlx, mlx_resizefunc func, void* param);

int main(int ac, char **av)
{
	t_fractol	*fractol;

	fractol = malloc (sizeof(t_fractol));
		if (fractol == 0)
			return (0);
	if (ac == 2)
	{
		if (strncmp(av[1], "Mandelbrot", 10) == 0 || strncmp(av[1], "mandelbrot", 10) == 0) // || av[2] == "mandelbrot")
		{
			fractol->mandelbrot = 1;
			fractol = initialize_fractol(fractol);
			if (fractol == NULL)
				return (EXIT_FAILURE);
		}
	}
	else if (ac == 4) //!needs an additional check if parameters are valid!
	{
		fractol->julia_r = atof(av[2]); //! take my own atoi
		fractol->julia_i = atof(av[3]);
		fractol = initialize_fractol(fractol);
			if (fractol == NULL)
				return (EXIT_FAILURE);
	}
	/* else
		{
			input_instructions();
			return (EXIT_FAILURE);
		} */
	return (EXIT_SUCCESS); //can I leave it here?!
}