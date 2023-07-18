/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 15:22:51 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/18 15:22:53 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	free_point(t_point *point)
{
	free(point->pos);
	free(point);
}

void	free_cursor(t_cursor *cursor)
{
	free_point(cursor->before_zoom);
	free_point(cursor->after_zoom);
	free(cursor);
}

void	free_memory(t_fractol *fractol)
{
	free_cursor(fractol->cursor);
	free(fractol);
}