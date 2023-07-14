/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zoom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 11:33:59 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/12 11:34:01 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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

//check what happens if mouse is outside of the window
//passing the address of the cursor variable to allow the mlx function
//to modify the value directly at the memory location
/* responsible for updating the cursor position based on the mouse 
coordinates retrieved from the mlx_get_mouse_pos function*/
void	store_cursor_position(t_fractol *fractol, t_point *cursor)
{
	free(cursor->pos);
	mlx_get_mouse_pos(fractol->window, &(cursor->x), &(cursor->y));
	cursor->pos = complex_n_conversion(cursor->x, cursor->y, fractol); 
}

//! do I need to make changes here for Julia?!
int	check_stability(t_cpx *z, t_cpx *c)
{
	t_cpx	*tmp;
	int			i;

	tmp = malloc (sizeof(t_cpx));
	if (tmp == NULL)
		return (0);
	i = 0;
	while (i < ITERATIONS)
	{
		tmp->real = (z->real * z->real - z->imag * z->imag) + c->real;
		tmp->imag = (2 * z->real * z->imag) + c->imag;
		z->real = tmp->real;
		z->imag = tmp->imag;
		if (z->real == INFINITY || z->imag == INFINITY
			|| !(z->real == z->real) || !(z->imag == z->imag))
		{
			free(tmp);
			return (i);
		}
		i++;
	}
	free(tmp);
	return (i);
}

/* function checks the value of ydelta to determine whether the zoom should
 be performed in or out*/
void	zoom_hook(double xdelta, double ydelta, void *param)
{
	t_fractol	*fractol;

	(void) xdelta;
	fractol = (t_fractol *) param;
	store_cursor_position(fractol, fractol->cursor->before_zoom);
	if (ydelta > 0)
	{
		fractol->zoom->type = OUT;
		fractol->zoom->value = fractol->zoom->value / ZOOM_FACTOR;
		fractol->zoom->shift = 1 + fractol->zoom->shift * ZOOM_FACTOR;
	}
	else if (ydelta < 0)
	{
		fractol->zoom->type = IN;
		fractol->zoom->value = fractol->zoom->value * ZOOM_FACTOR;
		fractol->zoom->shift = (fractol->zoom->shift - 1) / ZOOM_FACTOR;
	}
	store_cursor_position(fractol, fractol->cursor->after_zoom);
	color_fractol(fractol); //! I need to exchange this loop with mine!
	//mandelbrot_algo(fractol); //! this should just colour it
}