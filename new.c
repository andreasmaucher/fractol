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
				color = ft_pixel(n * 14, n * 7, n * 7, 42);
				mlx_put_pixel(fractol->image, x, y, color);
			}
			else 
				mlx_put_pixel(fractol->image, x, y, ft_pixel(0, 0, 0, 42));
}

/* MinRe, MaxRe, MinIm, MaxIm: defining boundaries, MaxIm is calculated based on the screen ratio, to avoid
image distortion when the display window isn't a square
Re_factor & Im_factor: used to map screen coordinates to complex numbers, calculated
by dividing the lenght of the rectangle with the screen width */
void mandelbrot_algo(t_fractol *fractol)
{
	double MinRe = -4.2;
	double MaxRe = 1.5;
	double MinIm = -1.5;
	double MaxIm = MinIm+(MaxRe-MinRe)*HEIGHT/WIDTH;
	double Re_factor = (MaxRe-MinRe)/(WIDTH-1);
	double Im_factor = (MaxIm-MinIm)/(HEIGHT-1);
	unsigned n=0;

	for(unsigned y=0; y<HEIGHT; ++y)
	{
		double c_im = MaxIm - y*Im_factor;
		for(unsigned x=0; x<WIDTH; ++x)
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
		}
	}
}

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
		fractol->zoom->value /= ZOOM_FACTOR;
		fractol->zoom->shift *= ZOOM_FACTOR;
	}
	else if (ydelta < 0)
	{
		fractol->zoom->type = IN;
		fractol->zoom->value *= ZOOM_FACTOR;
		fractol->zoom->shift /= ZOOM_FACTOR;
	}
	store_cursor_position(fractol, fractol->cursor->after_zoom);
	//color_fractol(fractol);
	mandelbrot_algo(fractol); //! this should just colour it
	(void) xdelta;
	(void) xdelta;
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

t_point	*initialize_point(double real, double imag)
{
	t_point		*point;

	point = malloc (sizeof(t_point));
	if (point == NULL)
		point = NULL;
	point->pos = initialize_complex(real, imag);
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
	mlx_loop_hook(fractol->window, ft_keys, fractol); //resposible for key movements
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