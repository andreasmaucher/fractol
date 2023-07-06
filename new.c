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
static double zoom = 1.0;

// -----------------------------------------------------------------------------

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void ft_randomize(void* param)
{
	t_fractol	*fractol;

	/*for (int32_t i = 0; i < fractol->image->width; ++i)
	{
		for (int32_t y = 0; y < fractol->image->height; ++y)
		{ */
			uint32_t color = ft_pixel(
				rand() % 0xFF, // R
				rand() % 0xFF, // G
				rand() % 0xFF, // B
				rand() % 0xFF  // A
			);
			//mlx_put_pixel(fractol->image, i, y, color);
		//}
	//}
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

/* does a number go to infinity or not?!*/
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
		//for zoom: c = move_fractol(c, fractol);
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

int mandelbrot(double cr, double ci)
{
    int i;
    double zr = 0.0, zi = 0.0, zr_new;

    for (i = 0; i < 1000; i++)
    {
        zr_new = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = zr_new;
        if (zr * zr + zi * zi > 4.0)
            return (i);
    }
    return (0);
}

static int32_t color_map[16] = {
    0x000000, // black
    0xFF0000, // red
    0xFF7F00, // orange
    0xFFFF00, // yellow
    0x00FF00, // green
    0x00FFFF, // cyan
    0x0000FF, // blue
    0xFF00FF, // magenta
    0x7F0000, // dark red
    0x7F3F00, // dark orange
    0x7F7F00, // dark yellow
    0x007F00, // dark green
    0x007F7F, // dark cyan
    0x00007F, // dark blue
    0x7F007F, // dark magenta
    0x7F7F7F  // gray
};

void mandelbrot_algo(t_fractol *fractol)
{
    uint32_t i;
	uint32_t j;
	int32_t iter;
	uint32_t color;
    double x, y;
	double size;
	//t_fractol *fractol;
	
	size = fmax(3.0 / (WIDTH * zoom), 2.0 / (HEIGHT * zoom));
    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            x = ((double)i / WIDTH) * 3.0 - 2.0;
            y = ((double)j / HEIGHT) * 3.0 - 1.5;
            iter = mandelbrot(x, y);
            if (iter > 0)
            {
				color = (iter < 16) ? color_map[iter] : 0x000000;
                mlx_put_pixel(fractol->image, i, j, color);
            }
            /* else
            {
                mlx_put_pixel(fractol->image, i, j, 0); //! adjust color
            } */
        }
    }
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

/* void	store_cursor_position(t_fractol *fractol, t_point *cursor)
{
	free(cursor->pos);
	mlx_get_mouse_pos(fractol->window, &(cursor->x), &(cursor->y));
	cursor->pos = from_mlx_to_complex(cursor->x, cursor->y, fractol);
} */

/* void	zoom_hook(double xdelta, double ydelta, void *param)
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
} */

/* handling arrow keys & escape button movements */
void ft_keys(void* param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *) param;
	if (mlx_is_key_down(fractol->window, MLX_KEY_ESCAPE))
		mlx_close_window(fractol->window);
	if (mlx_is_key_down(fractol->window, MLX_KEY_UP))
		image->instances[0].y -= 5;
	if (mlx_is_key_down(fractol->window, MLX_KEY_DOWN))
		image->instances[0].y += 5;
	if (mlx_is_key_down(fractol->window, MLX_KEY_LEFT))
		image->instances[0].x -= 5;
	if (mlx_is_key_down(fractol->window, MLX_KEY_RIGHT))
		image->instances[0].x += 5;
}

static void	set_hooks_and_loops(t_fractol *fractol)
{
	color_fractal(fractol);
	//mlx_scroll_hook(fractol->window, &zoom_hook, fractol);
	//mlx_loop_hook(fractol->window, keys_hook, fractol);
	mlx_resize_hook(fractol->window, 0, 0); //NULL NULL
	mlx_loop(fractol->window);
	mlx_terminate(fractol->window);
	//free_all(fractol);
}

void	ft_error(void)
{
	printf("%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

static	t_fractol	*initialize_fractol()//double x, double y
{
	t_fractol	*fractol;

	fractol = malloc (sizeof(t_fractol));
	if (fractol == 0)
		return (0);
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	mlx_image_to_window(fractol->window, fractol->image, 0, 0);
	/* if (!fractol->image || (mlx_image_to_window(fractol->window,
				fractol->image, 0, 0) == -1))
		ft_error(); */
	mandelbrot_algo(fractol);
	//mlx_loop_hook(fractol->window, mandelbrot_algo, fractol); //responsible for grey box
	mlx_loop_hook(fractol->window, ft_keys, fractol); //resposible for key movements

	mlx_loop(fractol->window); //keeps the window open until its closed by the user
	mlx_terminate(fractol->window);
	return (fractol);
}

int main() //int ac, char **av
{
	t_fractol	*fractol;

	fractol = initialize_fractol();
	return (EXIT_SUCCESS);
}