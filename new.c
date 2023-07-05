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

static mlx_image_t* image;

// -----------------------------------------------------------------------------

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void ft_randomize(void* param)
{
	for (int32_t i = 0; i < image->width; ++i)
	{
		for (int32_t y = 0; y < image->height; ++y)
		{
			uint32_t color = ft_pixel(
				rand() % 0xFF, // R
				rand() % 0xFF, // G
				rand() % 0xFF, // B
				rand() % 0xFF  // A
			);
			mlx_put_pixel(image, i, y, color);
		}
	}
}

void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		image->instances[0].y -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		image->instances[0].y += 5;
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		image->instances[0].x -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		image->instances[0].x += 5;
}

// -----------------------------------------------------------------------------

t_cpx	*move_fractol(t_cpx *num, t_fractol *fractol)
{
	double	diff_x;
	double	diff_y;

	diff_x = fractol->cursor->after_zoom->pos->real
		- fractol->cursor->before_zoom->pos->real;
	diff_y = fractol->cursor->after_zoom->pos->imag
		- fractol->cursor->before_zoom->pos->imag;
	if (fractol->zoom->type == IN)
	{
		num->real = num->real + (ZOOM_FACTOR * fractol->zoom->shift) * diff_x;
		num->imag = num->imag + (ZOOM_FACTOR * fractol->zoom->shift) * diff_y;
	}
	else if (fractol->zoom->type == OUT)
	{
		num->real = num->real - fractol->zoom->shift * diff_x;
		num->imag = num->imag - fractol->zoom->shift * diff_y;
	}
	return (num);
}

t_cpx	*from_mlx_to_complex(double x, double y, t_fractol *fractol)
{
	t_cpx	*num;

	num = malloc (sizeof(t_cpx));
	if (num == NULL)
		return (NULL);
	num->real = (-1 + 2 * (x / WIDTH)) * WIDTH / HEIGHT * fractol->zoom->value;
	num->imag = (1 - 2 * (y / HEIGHT)) * fractol->zoom->value;
	return (num);
}

int	check_stability(t_cpx *z, t_cpx *c)
{
	t_cpx	*tmp;
	int			i;

	tmp = malloc (sizeof(t_cpx));
	if (tmp == 0)
		return (0);
	i = 0;
	while (i < ITERATIONS)
	{
		tmp->real = (z->real * z->real - z->imag * z->imag) + c->real;
		tmp->imag = (2 * z->real * z->imag) + c->imag;
		z->real = tmp->real;
		z->imag = tmp->imag;
		if (z->real == INFINITY || z->imag == INFINITY
			|| !(z->real == z->real) || !(z->imag == z->imag))
		{
			free(tmp);
			return (i);
		}
		i++;
	}
	free(tmp);
	return (i);
}

t_cpx	*initialize_complex(double real, double imag)
{
	t_cpx	*compl;

	compl = malloc (sizeof(t_cpx));
	if (compl == 0)
		return (0);
	compl->real = real;
	compl->imag = imag;
	return (compl);
}

int	create_set(double x, double y, t_fractol *fractol)
{
	t_cpx	*z;
	t_cpx	*c;
	int			iterations;

		c = from_mlx_to_complex(x, y, fractol);
		c = move_fractol(c, fractol);
		z = initialize_complex(0, 0);
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
		color = ft_pixel(iter * 4, iter * 2, iter * 3, 58);
	else
		color = ft_pixel(0, 0, 0, 58);
	return (color);
}

void	color_fractal(t_fractol *fractol)
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

void	store_cursor_position(t_fractol *fractol, t_point *cursor)
{
	free(cursor->pos);
	mlx_get_mouse_pos(fractol->window, &(cursor->x), &(cursor->y));
	cursor->pos = from_mlx_to_complex(cursor->x, cursor->y, fractol);
}

void	zoom_hook(double xdelta, double ydelta, void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *) param;
	store_cursor_position(fractol, fractol->cursor->before_zoom);
	if (ydelta > 0)
	{
		fractol->zoom->type = OUT;
		fractol->zoom->value = fractol->zoom->value / ZOOM_FACTOR;
		fractol->zoom->shift = 1 + fractol->zoom->shift * ZOOM_FACTOR;
	}
	else if (ydelta < 0)
	{
		fractol->zoom->type = IN;
		fractol->zoom->value = fractol->zoom->value * ZOOM_FACTOR;
		fractol->zoom->shift = (fractol->zoom->shift - 1) / ZOOM_FACTOR;
	}
	store_cursor_position(fractol, fractol->cursor->after_zoom);
	color_fractal(fractol);
	(void) xdelta;
	(void) xdelta;
}

void	keys_hook(void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *) param;
	if (mlx_is_key_down(fractol->window, MLX_KEY_ESCAPE))
		mlx_close_window(fractol->window);
}

static void	set_hooks_and_loops(t_fractol *fractol)
{
	color_fractal(fractol);
	mlx_scroll_hook(fractol->window, &zoom_hook, fractol);
	mlx_loop_hook(fractol->window, keys_hook, fractol);
	mlx_resize_hook(fractol->window, 0, 0); //NULL NULL
	mlx_loop(fractol->window);
	mlx_terminate(fractol->window);
	//free_all(fractol);
}

static	t_fractol	*initialize_fractol()//double x, double y
{
	t_fractol	*fractol;

	fractol = malloc (sizeof(t_fractol));
	if (fractol == 0)
		return (0);
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	return (fractol);
}

int main() //int ac, char **av
{
	//mlx_t* mlx;
	t_fractol	*fractol;

	fractol = initialize_fractol();
	set_hooks_and_loops(fractol);
/* 
	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image = mlx_new_image(mlx, 128, 128)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	
	mlx_loop_hook(mlx, ft_randomize, mlx);
	mlx_loop_hook(mlx, ft_hook, mlx);

	mlx_loop(mlx);
	mlx_terminate(mlx); */
	return (EXIT_SUCCESS);
}