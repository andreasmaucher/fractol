// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

#include "fractol.h"

static mlx_image_t* image;

// -----------------------------------------------------------------------------

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void ft_randomize(void* param)
{
	for (int32_t i = 0; i < image->width; ++i)
	{
		for (int32_t y = 0; y < image->height; ++y)
		{
			uint32_t color = ft_pixel(
				rand() % 0xFF, // R
				rand() % 0xFF, // G
				rand() % 0xFF, // B
				rand() % 0xFF  // A
			);
			mlx_put_pixel(image, i, y, color);
		}
	}
}

void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
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

// -----------------------------------------------------------------------------

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

void mandelbrot_algo(void *param)
{
    int32_t i;
	int32_t j;
	int32_t iter;
	int32_t color;
    double x, y;

    for (i = 0; i < image->width; i++)
    {
        for (j = 0; j < image->height; j++)
        {
            x = ((double)i / image->width) * 3.0 - 2.0;
            y = ((double)j / image->height) * 3.0 - 1.5;
            iter = mandelbrot(x, y);
            if (iter > 0)
            {
				color = (iter < 16) ? color_map[iter] : 0x000000;
                mlx_put_pixel(image, i, j, color);
            }
            else
            {
                mlx_put_pixel(image, i, j, 0);
            }
        }
    }
}

int32_t main(int32_t argc, const char* argv[])
{
	mlx_t* mlx;

	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
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
	
	//mlx_loop_hook(mlx, ft_randomize, mlx);
	mlx_loop_hook(mlx, mandelbrot_algo, mlx);
	mlx_loop_hook(mlx, ft_hook, mlx);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
