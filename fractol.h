/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:20:46 by amaucher          #+#    #+#             */
/*   Updated: 2023/04/24 15:20:47 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MLX42/include/MLX42/MLX42.h"
#include "MLX42/include/MLX42/MLX42_Int.h"
#include "MLX42/include/MLX42/MLX42_Input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>

#define WIDTH 520
#define HEIGHT 520

typedef struct s_complex
{
	double	x;
	double	y;
}	        t_cpx;

struct color {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

typedef struct s_fractol
{
	char	m;
	char	s;
}			t_fractol;

#endif