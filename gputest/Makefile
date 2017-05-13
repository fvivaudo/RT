# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: momartin <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/06 20:44:24 by momartin          #+#    #+#              #
#    Updated: 2017/01/06 20:44:26 by momartin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gputest

CC = gcc
CFLAGS = -I ./srcs -I ./srcs/libft

SRCS = main.c init.c

LIBS = -L ./srcs/libft -lft -framework OpenCl

OBJ = $(addprefix ./srcs/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	make -C ./srcs/libft
	$(CC) $(OBJ) -o $(NAME) $(LIBS)
clean:
	make -C ./srcs/libft clean
	/bin/rm -f $(OBJ)

fclean: clean
	make -C ./srcs/libft fclean
	/bin/rm -f $(NAME)
re: fclean all

nolft:
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

test: $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)
	./gputest
