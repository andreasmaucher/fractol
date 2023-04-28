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

#define WIDTH 512
#define HEIGHT 512

static mlx_image_t* image;

// -----------------------------------------------------------------------------

/*this function combines the four RGBA values*/
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

/* ca & cb: Keep the original value for c, because of z^2 + c, meaning c
always has the same value */

/* int	ft_mandelbrot(int a, int b)
{
	int n;
	int max_iter;
	int	ca;
	int cb;
	int aa;
	int bb;

	ca = a;
	cb = b;

	max_iter = 100;
	
	while (n < 100)
	{
		aa = a * a - b * b;
		bb = 2 * a * b;
		a = aa + ca;
		b = bb + cb;
		if (abs(a + b) > 16) //!random limit
			break;
		n ++;
	}

}

int	main(int ac, char **av)
{
	int	a;
	int	b;
	int r;

	//! if ungleich Julia & Mandelbrot
	if (av[1] == "Mandelbrot" || av[1] == "mandelbrot")
	{
		r = ft_mandelbrot(a, b);
	}
} */

/*this creates the picture*/
void ft_randomize(void* param)
{
    int32_t x = 0;
    int32_t y = 0;
    
    while (x < image->width)
    {
        y = 0;
        while (y < image->height)
        {
            uint32_t color = ft_pixel(
            	0xFF, // R
                0xFF, // G
                0xFF, // B
                0xFF  // A
            );
            /*image, x, y, color*/
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
	/*generating picture*/
	mlx_loop_hook(mlx, ft_randomize, mlx);
	/*arrow key movements & escape*/
	mlx_loop_hook(mlx, ft_hook, mlx);
	/*loop until window is terminated*/
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
