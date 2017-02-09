# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/07/16 09:42:25 by fvivaudo          #+#    #+#              #
#    Updated: 2016/11/25 17:39:51 by fvivaudo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS = -Wall -Wextra -Werror -pthread

SRC = 	srcs/etc.c \
		srcs/init.c \
		srcs/init2.c \
		srcs/intersection.c \
		srcs/intersection2.c \
		srcs/main.c \
		srcs/perlin.c \
		srcs/vector.c

OBJ  = $(subst srcs/,,$(SRC:.c=.o))

NAME = RTv1

MLX =  -framework OpenGL -framework AppKit -lmlx -lm

LIBFT = -I libft/includes -L libft/ -lft 

all : $(NAME)

$(NAME) :
	@make -C minilibx_macos
	@make -C libft
	@gcc $(FLAGS) $(SRC) $(MLX) $(LIBFT) -I includes/ -o $(NAME)

clean:
	@rm -Rf $(OBJ)

fclean: clean
	@rm -Rf $(NAME)

re: fclean all
	
