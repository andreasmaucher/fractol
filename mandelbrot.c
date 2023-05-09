/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 19:21:06 by amaucher          #+#    #+#             */
/*   Updated: 2023/05/09 19:21:07 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/include/MLX42/MLX42.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 512
#define HEIGHT 512

static mlx_image_t* image;

int mandelbrot(double complex c)
{
    int i;
    double complex z = 0.0;

    for (i = 0; i < 1000; i++)
    {
        z = z*z + c;
        if (cabs(z) > 2.0)
            return (i);
    }
    return (0);
}

int main(void)
{
    void *mlx;
    void *win;
    int i, j, iter;
    double x, y;
    double complex c;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 800, 800, "Mandelbrot Set");

    for (i = 0; i < 800; i++)
    {
        for (j = 0; j < 800; j++)
        {
            x = ((double)i / 800.0) * 3.0 - 2.0;
            y = ((double)j / 800.0) * 3.0 - 1.5;
            c = x + y * I;
            iter = mandelbrot(c);

            if (iter > 0)
            {
                mlx_pixel_put(mlx, win, i, j, iter * 256);
            }
            else
            {
                mlx_pixel_put(mlx, win, i, j, 0);
            }
        }
    }

    mlx_loop(mlx);
    return (0);
}

