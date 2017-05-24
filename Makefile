# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/07/16 09:42:25 by fvivaudo          #+#    #+#              #
#    Updated: 2017/03/16 10:57:55 by dmoureu-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS = -Wall -Wextra -pthread -g

SRC = 	srcs/etc.c \
		srcs/init.c \
		srcs/intersection.c \
		srcs/main.c \
		srcs/perlin.c \
		srcs/vector.c \
		srcs/list.c \
		srcs/normal.c \
		srcs/iray.c \
		srcs/bump_mapping.c \
		srcs/blinn_phong.c \
		srcs/shadow.c \
		srcs/config.c \
		srcs/readconfig2.c


OBJ  = $(subst srcs/,,$(SRC:.c=.o))

NAME = RTv1

SDL2 =  -framework SDL2 -framework SDL2_image
SDL2_HEADER             = -I ~/Library/Frameworks/SDL2.framework/Headers
SDL2_IMAGE_HEADER       = -I ~/Library/Frameworks/SDL2_image.framework/Headers

SDL = -F ~/Library/Frameworks $(SDL2_MIXER) $(SDL2)
SDL_HEADER = -F ~/Library/Frameworks $(SDL2_IMAGE_HEADER) $(SDL2_HEADER)
INCLUDES_C = $(SDL) $(SDL_HEADER)
LIBFT = -I libft/includes -L libft/ -lft

all : $(NAME)

$(NAME) :
	#@make -C minilibx_macos
	@make -C libft
	@gcc $(FLAGS) $(SRC) $(SDL) $(LIBFT) $(INCLUDES_C) -flto -framework OpenCl -I includes/ -o $(NAME)

clean:
	@rm -Rf $(OBJ)

fclean: clean
	@rm -Rf $(NAME)

re: fclean all

sdl_install:
	curl https://dl.dropboxusercontent.com/u/22561204/SDL/Archive.zip > /tmp/Archive.zip
	unzip -o /tmp/Archive.zip -d ~/Library/Frameworks/
	curl https://dl.dropboxusercontent.com/content_link_zip/TMVqp4Z6JEk2gEO7F57rFVa3aPfzH5RFn6QIhBlCpBrMQ0btchhSCdRw3sty7Mar/file?dl=1 > /tmp/SDL2_image.zip
	unzip -o /tmp/SDL2_image.zip -d ~/Library/Frameworks/
	mv ~/Library/Frameworks/SDL2_image-2.0.1 ~/Library/Frameworks/SDL2_image.framework
