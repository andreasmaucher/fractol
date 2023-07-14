/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 11:01:45 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/11 11:01:47 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_zoom	*initialize_zoom(double value, double shift, bool type)
{
	t_zoom		*zoom;

	zoom = malloc (sizeof(t_zoom));
	if (zoom == NULL)
		zoom = NULL;
	zoom->value = value;
	zoom->shift = shift;
	zoom->type = type;
	return (zoom);
}

t_point	*initialize_point(double real, double imag)
{
	t_point		*point;

	point = malloc (sizeof(t_point));
	if (point == NULL)
		point = NULL;
	point->pos = initialization_cpx(real, imag);
	return (point);
}

//? why (0,0) -> because it"s only initialization and values are set later
t_cursor	*initialize_cursor(void)
{
	t_cursor		*cursor;

	cursor = malloc (sizeof(t_cursor));
	if (cursor == NULL)
		cursor = NULL;
	cursor->before_zoom = initialize_point(0, 0);
	cursor->after_zoom = initialize_point(0, 0);
	return (cursor);
}