#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: trichert <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/04 03:28:54 by trichert          #+#    #+#              #
#    Updated: 2016/12/14 19:48:50 by trichert         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME				= libft.a

SRC_PATH			= .
SRC_NAME			= ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c \
					ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_lstlen.c\
					ft_lstadd.c ft_lstdel.c ft_lstdelone.c ft_lstiter.c \
					ft_lstaddend.c \
					ft_lstmap.c ft_lstnew.c ft_memalloc.c ft_memccpy.c \
					ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memcpy_bck.c \
					ft_memdel.c ft_memmove.c ft_memset.c ft_putchar.c \
					ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c ft_putnbr.c \
					ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_strcat.c \
					ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c \
					ft_strdel.c ft_strdup.c ft_strequ.c ft_striter.c \
					ft_striteri.c ft_strjoin.c ft_strjoin.c ft_strlcat.c \
					ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c \
					ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c \
					ft_strnstr.c ft_strrchr.c ft_strsplit.c ft_strstr.c \
					ft_strsub.c ft_strtrim.c ft_tolower.c ft_toupper.c \
					ft_range.c ft_new_matrix.c ft_del_matrix.c ft_cpy_matrix.c \
					ft_set_mat_elem.c ft_get_mat_elem.c ft_transpose_mat.c \
					ft_sum_mat.c ft_mult_mat.c ft_itoa_base.c \
					ft_strtolower.c  ft_itoa_u.c ft_itoa_base_u.c \
					ft_putwchar.c

OBJ_PATH			= .
OBJ_NAME			= $(SRC_NAME:.c=.o)

SRC					= $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ					= $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

CFLAGS				= -Werror -Wall -Wextra -c
ARFLAG				= rc


CC					= /usr/bin/gcc
AR					= /usr/bin/ar
RANLIB				= /usr/bin/ranlib
CREATE_DIR			= /bin/mkdir
REMOVE				= /bin/rm -fv
REMOVE_DIR			= /bin/rm -rf

all: $(NAME)

$(NAME):
	@echo $(MSG_BEGIN)
	@echo $(MSG_PREPOC)
	$(CC) $(CFLAGS) $(SRC)
	@echo $(MSG_COMPILING) $(MSG_DODALIB)
	$(AR) $(ARFLAG) $(NAME) $(OBJ)
	$(RANLIB) $(NAME)
	@echo $(MSG_END)

clean:
	@echo $(MSG_CLEAN)
	$(REMOVE) $(OBJ)


fclean: clean
	@echo $(MSG_CLEANING)
	$(REMOVE) $(NAME)
	@echo $(MSG_END)

re: fclean all

MSG_BEGIN			= '---- BEGIN ----'
MSG_END				= '---- END ----'
MSG_CLEAN			= '---- CLEAN ----'
MSG_PREPROC			= 'Preprocessing:'
MSG_COMPILING		= 'Compiling :'
MSG_DODALIB			= 'Creating the lib'
MSG_CLEANING		= 'Cleaning project folder'
MSG_SAVING			= 'Saving files for backup'
