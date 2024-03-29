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

/*setting up all hooks and loops provided by the MLX42 library*/
static	t_fractol	*setup_hooks(t_fractol *fractol)
{
	mlx_scroll_hook(fractol->window, &zoom_hook, fractol);
	mlx_loop_hook(fractol->window, ft_escape_key, fractol);
	mlx_loop(fractol->window);
	mlx_terminate(fractol->window);
	return (fractol);
}

/*initializes the fractol struct and calls all functions necessary to
generate the fractol and finally free the memory*/
static	t_fractol	*generate_fractol(char *set_name, char *x, char *y)
{
	t_fractol	*fractol;

	fractol = malloc(sizeof(t_fractol));
	if (fractol == NULL)
		return (NULL);
	initialize_mlx(fractol);
	fractol->cursor = initialize_cursor();
	set_specific_initialization(fractol, set_name, x, y);
	fractol->zoom_level = 1;
	fractol->zoom_centered = 0;
	fractol->zoom_in_out = 2;
	color_fractol(fractol);
	setup_hooks(fractol);
	free_memory(fractol);
	return (0);
}

/*error checks the input string*/
void	setup_mandelbrot(char *set_name, char *x, char *y)
{
	if (ft_strncmp(set_name, "Mandelbrot", 10) == 0
		|| ft_strncmp(set_name, "mandelbrot", 10) == 0)
		generate_fractol(set_name, x, y);
	else
		input_instructions();
}

/*error checks the input string*/
void	setup_julia(char *set_name, char *x, char *y)
{
	if (ft_strncmp(set_name, "Julia", 5) == 0
		|| ft_strncmp(set_name, "julia", 5) == 0)
	{
		if (x[ft_strlen(x) - 1] == '.' || y[ft_strlen(y) - 1] == '.')
			input_instructions();
		else if (check_if_num(x) && check_if_num(y))
			generate_fractol(set_name, x, y);
		else
			input_instructions();
	}
}

int	main(int ac, char **av)
{
	char		*set_name;
	char		*x;
	char		*y;

	set_name = av[1];
	if (ac == 2)
	{
		x = NULL;
		y = NULL;
		setup_mandelbrot(set_name, x, y);
	}
	else if (ac == 4)
	{
		x = av[2];
		y = av[3];
		setup_julia(set_name, x, y);
	}
	else
		return (input_instructions());
	return (EXIT_SUCCESS);
}
