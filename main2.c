/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 15:02:57 by amaucher          #+#    #+#             */
/*   Updated: 2023/04/27 15:02:58 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH 128
#define HEIGHT 128

static mlx_image_t* image;

// -----------------------------------------------------------------------------

/*this function combines the four RGBA values*/
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

/* ca & cb: Keep the original value for c, because of z^2 + c, meaning c
always has the same value */

uint32_t	ft_mandelbrot(int a, int b, int n)
{
	float	ca;
	float cb;
	float aa;
	float bb;

	ca = a;
	cb = b;
	n = 0;
	
	while (n < 100)
	{
		aa = a * a - b * b;
		bb = 2 * a * b;
		a = aa + ca;
		b = bb + cb;
		if (a * a + b * b > 16) //!random limit
			break;
		n ++;
	}
	return (n);
}

float map(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
    // Calculate the input value as a fraction of the input range
    float inputFraction = (value - inputMin) / (inputMax - inputMin);

    // Scale the input fraction to the output range and add the output minimum
    float outputValue = outputMin + (inputFraction * (outputMax - outputMin));

    // Return the mapped value
    return outputValue;
}

/*this creates the picture*/
void ft_randomize(void* param)
{
    int32_t x = 0;
    int32_t y = 0;
	uint32_t color;
	int n; //number of iterations

    while (x < image->width)
    {
        y = 0;
        while (y < image->height)
        {
			float a = map(x, 0, WIDTH, -2.5, 2.5);
			float b = map(y, 0, HEIGHT, -2.5, 2.5);
			n = ft_mandelbrot(a, b, n);
			if (n == 100)
            	color = ft_pixel(0xFF, 100, 0xFF, 0xFF);
			else 
            	color = ft_pixel(0xFF, 0xFF, 0xFF, 0xFF);
            /*puts the pixels on the image; image, x, y, color*/
            mlx_put_pixel(image, x, y, color);
            y++;
        }
        x++;
    }
}

/* for moves with the arrow keys */
void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE)) //closes window!
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

int32_t main(int32_t argc, const char* argv[])
{
	mlx_t* mlx;

	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	/*determines the size of the image*/
	if (!(image = mlx_new_image(mlx, WIDTH, HEIGHT)))
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
	/*generating picture*/
	mlx_loop_hook(mlx, ft_randomize, mlx);
	/*arrow key movements & escape*/
	mlx_loop_hook(mlx, ft_hook, mlx);
	/*loop until window is terminated*/
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
