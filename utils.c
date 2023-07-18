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
void	ft_escape_key(void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *) param;
	if (mlx_is_key_down(fractol->window, MLX_KEY_ESCAPE))
		mlx_close_window(fractol->window);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char) s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	input_instructions(void)
{
	ft_printf("Valid set names are 'Mandelbrot' or 'Julia'. \n");
	ft_printf("For Julia sets add coordinates as follows: 'Julia -0.42 0.69'");
	return (EXIT_FAILURE);
}
