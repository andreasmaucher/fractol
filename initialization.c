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

t_cpx	*initialization_cpx(double real, double imag)
{
	t_cpx	*cpx;

	cpx = malloc (sizeof(t_cpx));
	if (cpx == NULL)
		return (NULL);
	cpx->real = real;
	cpx->imag = imag;
	return (cpx);
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
