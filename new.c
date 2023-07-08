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
    0x421E0F,
	0x19071A,
	0x09012F,
	0x040449,
	0x000764,
	0x0C2C8A,
	0x1852B1,
	0x397DD1,
	0x86B5E5,
	0xD3ECF8,
	0xF1E9BF,
	0xF8C95F,
	0xFFAA00,
	0xCC8000,
	0x995700,
	0x6A3403,
};

void mandelbrot_algo(t_fractol *fractol)
{
	double MinRe = -4.2;
	double MaxRe = 1.5;
	double MinIm = -1.5;
	double MaxIm = MinIm+(MaxRe-MinRe)*HEIGHT/WIDTH;
	double Re_factor = (MaxRe-MinRe)/(WIDTH-1);
	double Im_factor = (MaxIm-MinIm)/(HEIGHT-1);
	unsigned n=0;
	uint32_t color;

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
			if (n < ITERATIONS)
			{
				int i = n % 16;
				//color = (n < 16) ? color_map[n] : 0x000000;
				color = ft_pixel(n * 14, n * 7, n * 7, 42);
				mlx_put_pixel(fractol->image, x, y, color);
			}
			else 
				mlx_put_pixel(fractol->image, x, y, ft_pixel(0, 0, 0, 42));
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

static	t_fractol	*initialize_fractol()//double x, double y
{
	t_fractol	*fractol;

	fractol = malloc (sizeof(t_fractol));
	if (fractol == 0)
		return (0);
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	mlx_image_to_window(fractol->window, fractol->image, 0, 0);
	mandelbrot_algo(fractol);
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