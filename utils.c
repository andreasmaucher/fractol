/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 11:06:38 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/13 11:06:40 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/* handling arrow keys & escape button movements, void param necessary
for compatibility with mlx functions */
void ft_escape_key(void* param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *) param;
	if (mlx_is_key_down(fractol->window, MLX_KEY_ESCAPE))
		mlx_close_window(fractol->window);
}