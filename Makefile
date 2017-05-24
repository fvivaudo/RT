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
		srcs/vector.c \
		srcs/vector1.c \
		srcs/vector2.c \
		srcs/vector3.c \
		srcs/list.c \
		srcs/config.c \
		srcs/readconfig2.c \
		srcs/readconfig2_1.c \
		srcs/readconfig2_2.c \
		srcs/readconfig2_3.c \
		srcs/readconfig2_4.c \
		srcs/config1.c \
		srcs/config2.c \
		srcs/config3.c \
		srcs/config4.c \
		srcs/config5.c \
		srcs/config6.c \
		srcs/config7.c \
		srcs/engine1.c \
		srcs/engine2.c \
		srcs/engine3.c \
		srcs/engine4.c \
		srcs/engine5.c \
		srcs/engine6.c \
		srcs/engine7.c \
		srcs/engine8.c \

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
