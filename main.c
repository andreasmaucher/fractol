// -----------------------------------------------------------------------------
// Codam Coding College, Amsterdam @ 2022-2023 by W2Wizard.
// See README in the root project for more information.
// -----------------------------------------------------------------------------

#include "fractol.h"

//implement dynamic window sizing & scrolling

static mlx_image_t* image;
static double zoom = 1.0;  // Initial zoom level



// -----------------------------------------------------------------------------

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

/* void ft_hook(void* param)
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
} */

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
   /*  if (mlx_is_mouse_scroll_up(mlx))
    {
        image->instances[0].x += image->width / 4;
        image->instances[0].y += image->height / 4;
        zoom *= 1.1;
    }
    if (mlx_is_mouse_scroll_down(mlx))
    {
        image->instances[0].x -= image->width / 4;
        image->instances[0].y -= image->height / 4;
        zoom *= 0.9;
    } */
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

/* map the pixel coordinates (i, j) to the corresponding 
complex coordinates (x, y)*/
void mandelbrot_algo()
{
    uint32_t i;
	uint32_t j;
	int32_t iter;
	int32_t color;
    double x, y;
	double size;
	
	size = fmax(3.0 / (WIDTH * zoom), 2.0 / (HEIGHT * zoom));
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
                mlx_put_pixel(image, i, j, 0); //! adjust color
            }
        }
    }
}

/* void	scroll_handler(double xdelta, double ydelta, mlx_t* mlx)
{
	zoom += ydelta * 0.1;
	zoom += xdelta * 0.1; //!not sure if really needed
	mandelbrot_algo();
	mlx_image_to_window(mlx, image, 0, 0);
} */

void my_scrollhook(double xdelta, double ydelta, void *param)
{
	// Simple up or down detection.
	/* if (ydelta > 0)
		puts("Up!");
	else if (ydelta < 0)
		puts("Down!"); */
	(void)param;
	(void)xdelta;
	if (ydelta > 0)
		zoom += ydelta * 0.1;
	//zoom += xdelta * 0.1; //!not sure if really needed
	mandelbrot_algo();
	//mlx_image_to_window(mlx, image, 0, 0);
}

static void set_hooks_and_loops(t_fractol *fractol)
{
	color_fractol(fractol);
	mlx_scroll_hook(fractol->window, &zoom_hook, fractol);
	mlx_loop_hook(fractol->window, keys_hook, fractol);
	mlx_resize_hook(fractol->window, NULL, NULL);
	mlx_loop(fractol->window);
	mlx_terminate(fractol->window);
	free_all(fractol);
}

// only needs argv input for the Julia set
t_fractol *initialize_window(char *set_type) //double x, double y only needed for Julia
{
	t_fractol	*fractol;

	fractol = malloc(sizeof(t_fractol));
	if (fractol == NULL)
		return (NULL);
	//initializes a new MLX instance
	fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true);
	//creates and allocates a new image buffer
	fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT);
	fractol->set = initialize_set(set_type); //! not working because I have different structure
	/* if (!(fractol->window = mlx_init(WIDTH, HEIGHT, "fractol", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_SUCCESS);
	}
	if (!(fractol->image = mlx_new_image(fractol->window, WIDTH, HEIGHT)))
	{
		mlx_close_window(fractol->window);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_SUCCESS);
	}
	if (mlx_image_to_window(fractol->window, fractol->image, 0, 0) == -1)
	{
		mlx_close_window(fractol->window);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_SUCCESS);
	} */
	return(fractol);
}

int main(void)
{
	t_fractol	*fractol;

	fractol = initialize_window(argv[1]);
	set_hooks_&_loops(fractol);
	/* mlx_loop_hook(fractol->window, mandelbrot_algo, fractol);
	mlx_loop_hook(fractol->window, ft_hook, fractol);
	//mlx_scroll_hook(fractol->window, &my_scrollhook, NULL);
	mlx_loop(fractol->window);
	mlx_terminate(fractol->window); */
	return (EXIT_SUCCESS);
}