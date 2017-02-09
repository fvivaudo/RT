# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/07/16 09:42:25 by fvivaudo          #+#    #+#              #
#    Updated: 2016/11/02 11:15:52 by fvivaudo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS = -Wall -Werror -Wextra

SRC =	srcs/flags.c		\
		srcs/ft_abs.c       \
		srcs/ft_atoi.c       \
		srcs/ft_bzero.c		\
		srcs/ft_chitoa.c		\
		srcs/ft_cos.c		\
		srcs/ft_count.c		\
		srcs/ft_doubletabcpy.c		\
		srcs/ft_doubletabfree.c		\
		srcs/ft_datoi.c       \
		srcs/ft_hexa.c		\
		srcs/ft_isalpha.c	\
		srcs/ft_isalnum.c	\
		srcs/ft_isascii.c	\
		srcs/ft_isdigit.c	\
		srcs/ft_ishere.c \
		srcs/ft_isheren.c	\
		srcs/ft_isprint.c	\
		srcs/ft_ldiff.c	\
		srcs/ft_itoa.c		\
		srcs/ft_litoa.c		\
		srcs/ft_llitoa.c	\
		srcs/ft_lndiff.c	\
		srcs/ft_lstadd.c		\
		srcs/ft_lstnadd.c	\
		srcs/ft_lstaddb.c	\
		srcs/ft_lstdel.c		\
		srcs/ft_lstdelone.c	\
		srcs/ft_lstndelone.c	\
		srcs/ft_lstget.c		\
		srcs/ft_lstiter.c	\
		srcs/ft_lstmap.c		\
		srcs/ft_lstnew.c		\
		srcs/ft_lstrev.c		\
		srcs/ft_memalloc.c	\
		srcs/ft_memcatf.c	\
		srcs/ft_memccpy.c	\
		srcs/ft_memchr.c		\
		srcs/ft_memcmp.c		\
		srcs/ft_memcpy.c		\
		srcs/ft_memdel.c		\
		srcs/ft_memmove.c	\
		srcs/ft_memset.c		\
		srcs/ft_ncount.c		\
		srcs/ft_octal.c	\
		srcs/ft_pow.c	\
		srcs/ft_putchar.c	\
		srcs/ft_putchar_fd.c	\
		srcs/ft_putendl.c	\
		srcs/ft_putendl_fd.c	\
		srcs/ft_putnbr.c		\
		srcs/ft_putnbr_fd.c	\
		srcs/ft_putstr.c		\
		srcs/ft_putstr_fd.c	\
		srcs/ft_printf.c	\
		srcs/ft_root.c	\
		srcs/ft_sin.c		\
		srcs/ft_strcat.c		\
		srcs/ft_strcatf.c		\
		srcs/ft_strchr.c		\
		srcs/ft_strcpy.c		\
		srcs/ft_strclr.c		\
		srcs/ft_strcmp.c		\
		srcs/ft_strdel.c		\
		srcs/ft_strdup.c		\
		srcs/ft_strndup.c	\
		srcs/ft_strext.c		\
		srcs/ft_strequ.c		\
		srcs/ft_striter.c	\
		srcs/ft_striteri.c	\
		srcs/ft_strjoin.c	\
		srcs/ft_strlcat.c	\
		srcs/ft_strlcpy.c	\
		srcs/ft_strlen.c		\
		srcs/ft_strmap.c		\
		srcs/ft_strmapi.c	\
		srcs/ft_strncat.c	\
		srcs/ft_strncatf.c	\
		srcs/ft_strncmp.c	\
		srcs/ft_strncpy.c	\
		srcs/ft_strnequ.c	\
		srcs/ft_strnew.c		\
		srcs/ft_strnstr.c	\
		srcs/ft_strrchr.c	\
		srcs/ft_strsplit.c	\
		srcs/ft_strsplitline.c	\
		srcs/ft_strsplitspace.c	\
		srcs/ft_strstr.c		\
		srcs/ft_strsub.c		\
		srcs/ft_strtrim.c	\
		srcs/ft_strup.c	\
		srcs/ft_tan.c		\
		srcs/ft_tolower.c	\
		srcs/ft_toupper.c	\
		srcs/ft_uns.c		\
		srcs/getlast.c		\
		srcs/get_next_line.c	\
		srcs/opt1.c		\
		srcs/opt2.c		\
		srcs/opt3.c		\
		srcs/opt4.c		\
		srcs/opt5.c		\
		srcs/order.c		\
		srcs/recursive.c

OBJ  = $(subst srcs/,,$(SRC:.c=.o))

NAME = libft.a

all : $(NAME)

$(NAME) :
	@gcc  -c $(FLAGS) $(SRC) -I includes/
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)

clean:
	@rm -Rf $(OBJ)

fclean: clean
	@rm -Rf $(NAME)

re: fclean all
	
