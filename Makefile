# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amaucher <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/24 15:20:36 by amaucher          #+#    #+#              #
#    Updated: 2023/04/24 15:20:38 by amaucher         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fractol
SOURCES = main.c zoom.c initialization.c color.c utils.c math.c input_check.c \
			ft_split.c memory_management.c
FT_PRINTF = ft_printf/ft_printf.a
MLX42 = MLX42/build/libmlx42.a
OBJS = $(SOURCES:.c=.o)
INCLUDES = -I ft_printf/includes -I MLX42/include

CC = cc -g
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
MLXFLAGS = -ldl -lglfw -pthread -lm

all: $(NAME)	

$(NAME): $(FT_PRINTF) $(MLX42) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(FT_PRINTF) $(MLX42) $(MLXFLAGS) $(INCLUDES) -o $(NAME)

$(FT_PRINTF):
	make -C ft_printf

$(MLX42):
	make -C MLX42

clean:
	make -C ft_printf clean
	$(RM) $(OBJS)

fclean: clean
	make -C ft_printf clean
	$(RM) $(NAME)

re: fclean $(NAME)
