/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:20:46 by amaucher          #+#    #+#             */
/*   Updated: 2023/04/24 15:20:47 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>
#include <stddef.h>
#include <string.h>
#include "./MLX42/include/MLX42/MLX42.h"

/* defines the size of the window */
#define WIDTH 1920
#define HEIGHT 980

#define ITERATIONS 142
#define ZOOM_FACTOR 0.69

/* parameters for zooming in and out */
#define IN 0
#define OUT 1

typedef struct s_complex
{
	double	real;
	double	imag;
}	        t_cpx;

typedef struct s_point
{
	int32_t		x;
	int32_t		y;
	t_cpx	*pos;
}	t_point;

typedef struct s_cursor
{
	t_point	*before_zoom;
	t_point	*after_zoom;
}	t_cursor;

typedef struct s_fractol
{
	mlx_t		*window;
	mlx_image_t	*image;
	t_cursor	*cursor;
	int			mandelbrot;
	double			julia_i;
	double			julia_r;
	double	value;
	bool	type;
	double	shift;
}			t_fractol;

void	zoom_hook(double xdelta, double ydelta, void *param);
t_cpx	*initialization_cpx(double real, double imag);
t_fractol	*initialize_zoom(double value, double shift, bool type);
t_cursor	*initialize_cursor(void);
t_point	*initialize_point(double real, double imag);

/*zoom*/
t_cpx	*zoom_fractol(t_cpx *num, t_fractol *fractol);
void	store_cursor_position(t_fractol *fractol, t_point *cursor);
int	check_stability(t_cpx *z, t_cpx *c);
void	zoom_hook(double xdelta, double ydelta, void *param);

/*math*/
t_cpx	*complex_n_conversion(double x, double y, t_fractol *fractol);

/*color*/
void	color_fractol(t_fractol *fractol);
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
void	ft_colouring(t_fractol *fractol, unsigned n, unsigned x, unsigned y);

/*utils*/
void ft_escape_key(void* param);

/*input check*/
double	calc_fractional_part(char *str);
double string_to_float(char *str);
double	check_if_num(char *str);

/*ft_split*/
char	**ft_split(char const *s, char c);

#endif