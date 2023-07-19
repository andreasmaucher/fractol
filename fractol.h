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

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <complex.h>
# include <math.h>
# include <stddef.h>
# include <string.h>
# include "./MLX42/include/MLX42/MLX42.h"
# include "./ft_printf/ft_printf.h"

/* defines the size of the window */
# define WIDTH 1920
# define HEIGHT 980

/*iterations determine the level of detail and complexity and refer to
 the number of times a specific mathematical operation is repeated to 
 generate the fractal pattern*/
# define ITERATIONS 142

/* the closer to 1 the more granular zoom */
# define ZOOM_FACTOR 0.69

/* parameters for zooming in and out */
# define IN 0
# define OUT 1

/*struct for complex numbers*/
typedef struct s_complex
{
	double	real;
	double	imag;
}	t_cpx;

/*struct that determines the position of a certain point on the window*/
typedef struct s_point
{
	int32_t		x;
	int32_t		y;
	t_cpx		*pos;
}	t_point;

/*struct used to store the position of the cursor*/
typedef struct s_cursor
{
	t_point	*before_zoom;
	t_point	*after_zoom;
}	t_cursor;

/*value, type & shift are used for zoom*/
typedef struct s_fractol
{
	mlx_t		*window;
	mlx_image_t	*image;
	t_cursor	*cursor;
	int			mandelbrot;
	double		julia_i;
	double		julia_r;
	double		value;
	bool		type;
	double		shift;
}	t_fractol;

/*initialization*/
t_cpx		*initialization_cpx(double real, double imag);
t_fractol	*initialize_zoom(double value, double shift, bool type);
t_cursor	*initialize_cursor(void);
t_point		*initialize_point(double real, double imag);
void		set_specific_initialization(t_fractol *fractol, char *set_name,
				char *x, char *y);
int			initialize_mlx(t_fractol *fractol);

/*zoom*/
t_cpx		*zoom_fractol(t_cpx *num, t_fractol *fractol);
void		store_cursor_position(t_fractol *fractol, t_point *cursor);
int			check_stability(t_cpx *z, t_cpx *c);
void		zoom_hook(double xdelta, double ydelta, void *param);

/*math*/
t_cpx		*complex_n_conversion(double x, double y, t_fractol *fractol);
int			create_fractol(double x, double y, t_fractol *fractol);

/*color*/
void		color_fractol(t_fractol *fractol);
int32_t		ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
uint32_t	color_set(double x, double y, t_fractol *fractol);

/*utils*/
void		ft_escape_key(void *param);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		free_memory(t_fractol *fractol);
int			input_instructions(void);
void		free_split_str(char **split_str);

/*input check*/
double		calc_fractional_part(char *str);
double		string_to_float(char *str);
double		check_if_num(char *str);

/*ft_split*/
char		**ft_split(char const *s, char c);

#endif