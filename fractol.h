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
#include "./MLX42/include/MLX42/MLX42.h"

/* defines the size of the window */
#define WIDTH 1920
#define HEIGHT 980

#define ITERATIONS 100
#define ZOOM_FACTOR 0.7

/* parameters for zooming in and out */
#define IN 0
#define OUT 1

#define START 2

typedef struct s_complex
{
	double	real;
	double	imag;
}	        t_cpx;

struct color 
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

typedef struct s_zoom
{
	double	value;
	bool	type;
	double	shift;
}	t_zoom;

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

typedef struct s_set
{
	char		*type;
	t_cpx		*origin;
}	t_set;

typedef struct s_fractol
{
	mlx_t		*window;
	mlx_image_t	*image;
	char		*type;
	t_cpx		*origin;
	t_set			*set;
	t_zoom		*zoom;
	t_cursor	*cursor;
}			t_fractol;

#endif