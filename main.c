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

void	ft_colouring(t_fractol *fractol, unsigned n, unsigned x, unsigned y)
{
	uint32_t color;

	if (n < ITERATIONS)
			{
				int i = n % 16;
				color = ft_pixel(n * 42, n * 84, n * 21, 84);
				mlx_put_pixel(fractol->image, x, y, color);
			}
	else 
		mlx_put_pixel(fractol->image, x, y, ft_pixel(0, 0, 0, 42));
}

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
			double c_re = MinRe + x*Re_factor;

			double Z_re = c_re, Z_im = c_im;
			n = 0;
			while(n < ITERATIONS)  //after this loop we get value between 0 and max iter
			{
				double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
				if(Z_re2 + Z_im2 > 4)
					break;
				Z_im = 2*Z_re*Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
				n++;
			}
			ft_colouring(fractol, n, x, y);
			++x;
		}
		++y;
	}
}

/* handling arrow keys & escape button movements */
void ft_escape_key(void* param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *) param;
	if (mlx_is_key_down(fractol->window, MLX_KEY_ESCAPE))
		mlx_close_window(fractol->window);
}

/* responsible for converting the mouse coordinates (x and y) to 
complex number coordinates based on the window dimensions and the zoom value*/
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

//check what happens if mouse is outside of the window
//passing the address of the cursor variable to allow the mlx function
//to modify the value directly at the memory location
/* responsible for updating the cursor position based on the mouse 
coordinates retrieved from the mlx_get_mouse_pos function*/
void	store_cursor_position(t_fractol *fractol, t_point *cursor)
{
	free(cursor->pos);
	mlx_get_mouse_pos(fractol->window, &(cursor->x), &(cursor->y));
	cursor->pos = from_mlx_to_complex(cursor->x, cursor->y, fractol); 
}

t_cpx	*initialize_complex(double real, double imag)
{
	t_cpx	*compl;

	compl = malloc (sizeof(t_cpx));
	if (compl == NULL)
		return (NULL);
	compl->real = real;
	compl->imag = imag;
	return (compl);
}

int	check_stability(t_cpx *z, t_cpx *c)
{
	t_cpx	*tmp;
	int			i;

	tmp = malloc (sizeof(t_cpx));
	if (tmp == NULL)
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

/* function checks the value of ydelta to determine whether the zoom should
 be performed in or out*/
void	zoom_hook(double xdelta, double ydelta, void *param)
{
	t_fractol	*fractol;

	(void) xdelta;
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
	color_fractol(fractol); //! I need to exchange this loop with mine!
	//mandelbrot_algo(fractol); //! this should just colour it
}

t_zoom	*initialize_zoom(double value, double shift, bool type)
{
	t_zoom		*zoom;

	zoom = malloc (sizeof(t_zoom));
	if (zoom == NULL)
		zoom = NULL;
	zoom->value = value;
	zoom->shift = shift;
	zoom->type = type;
	return (zoom);
}

t_point	*initialize_point(double real, double imag)
{
	t_point		*point;

	point = malloc (sizeof(t_point));
	if (point == NULL)
		point = NULL;
	point->pos = initialize_complex(real, imag);
	return (point);
}

//? why (0,0) -> because it"s only initialization and values are set later
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

static	t_fractol	*initialize_fractol()//double x, double y
{
	t_fractol	*fractol;

	fractol = malloc (sizeof(t_fractol));
	if (fractol == 0)
		return (0);
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	mlx_image_to_window(fractol->window, fractol->image, 0, 0);
	fractol->zoom = initialize_zoom(1, 0, START); //!ZOOM
	fractol->cursor = initialize_cursor();
	mandelbrot_algo(fractol);
	mlx_scroll_hook(fractol->window, &zoom_hook, fractol); //!ZOOM
	mlx_loop_hook(fractol->window, ft_escape_key, fractol); //resposible for key movements
	//mlx_resize_hook(fractol->window, ft_resize, fractol);
	mlx_loop(fractol->window); //keeps the window open until its closed by the user
	mlx_terminate(fractol->window);
	return (fractol);
}

//typedef void (*mlx_resizefunc)(int32_t width, int32_t height, void* param);
//void mlx_resize_hook(mlx_t* mlx, mlx_resizefunc func, void* param);

int main() //int ac, char **av
{
	t_fractol	*fractol;

	fractol = initialize_fractol();
	if (fractol == NULL)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}