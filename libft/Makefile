# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/02 14:09:36 by galambey          #+#    #+#              #
#    Updated: 2023/12/22 17:30:18 by athiebau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC = src/print/ft_putchar_fd.c \
      src/print/ft_putstr_fd.c \
      src/print/ft_putendl_fd.c \
      src/print/ft_putnbr_fd.c \
      src/print/get_next_line/get_next_line.c \
      src/print/get_next_line/get_next_line_multiple_fd.c \
      src/print/get_next_line/get_next_line_utils.c \
	src/print/ft_printf/ft_printf.c \
	src/print/ft_printf/do_print.c \
      src/print/ft_printf/manage_arg.c \
      src/print/ft_printf/ft_atoi_printf.c \
      src/print/ft_printf/ft_print_char.c \
      src/print/ft_printf/ft_print_int.c \
      src/print/ft_printf/check_flag.c \
      src/print/ft_printf/ft_print_unsignedint.c \
      src/print/ft_printf/ft_print_lowhexa.c \
      src/print/ft_printf/ft_print_upphexa.c \
      src/print/ft_printf/ft_print_str.c \
      src/print/ft_printf/ft_print_ptr.c \
      \
      src/number/ft_atoi.c \
      src/number/ft_abs.c \
      src/number/ft_dabs.c \
      src/number/ft_fabs.c \
      src/number/ft_itoa.c \
      src/number/digit_len.c \
      src/number/ft_sign.c \
      src/number/ft_swap_int.c \
      \
      src/string/ft_strlen.c \
	src/string/ft_strlen_bfr_char.c \
      src/string/ft_strlen_afr_char.c \
      src/string/ft_arraylen.c \
	  src/string/ft_strcpy.c \
      src/string/ft_strlcpy.c \
      src/string/ft_strcat.c \
      src/string/ft_strlcat.c \
      src/string/ft_strncmp.c \
      src/string/ft_split.c \
      src/string/ft_split_isspace.c \
      src/string/ft_free_split.c \
      src/string/ft_strjoin.c \
	src/string/ft_strjoin_bfr_char.c \
      src/string/ft_strjoin_aftr_char.c \
	  src/string/ft_strjoin_char.c \
      src/string/ft_toupper.c \
      src/string/ft_tolower.c \
      src/string/ft_strchr.c \
      src/string/ft_strrchr.c \
      src/string/ft_strcmp.c \
      src/string/ft_strnstr.c \
      src/string/ft_strtrim.c \
      src/string/ft_strtrim_exept_set.c \
	  src/string/ft_strtrim_except_tips.c \
      src/string/ft_substr.c \
      src/string/ft_striteri.c \
      src/string/ft_strmapi.c \
      src/string/ft_is_char.c \
      src/string/ck_char.c \
      src/string/ck_sset.c \
	  src/string/ft_random_string.c \
      src/string/ft_replace_char.c \
      \
      src/test/ft_isalpha.c \
      src/test/ft_isdigit.c \
      src/test/ft_isalnum.c \
      src/test/ft_isascii.c \
      src/test/ft_isprint.c \
      src/test/ft_isupp.c \
      src/test/ft_islower.c \
      \
      src/memmory/ft_memset.c \
      src/memmory/ft_memchr.c \
      src/memmory/ft_memcpy.c \
      src/memmory/ft_memcmp.c \
      src/memmory/ft_bzero.c \
      src/memmory/ft_memmove.c \
      \
      src/alloc/ft_calloc.c \
      src/alloc/ft_strdup.c \
      \
      src/linked_list/ft_lstadd_front.c \
      src/linked_list/ft_lstnew.c \
      src/linked_list/ft_lstsize.c \
      src/linked_list/ft_lstadd_back.c \
      src/linked_list/ft_lstlast.c \
      src/linked_list/ft_lstdelone.c \
      src/linked_list/ft_lstclear.c \
      src/linked_list/ft_lstiter.c \
      src/linked_list/ft_lstmap.c \
      \
      src/double_linked_list/lst_two_addnew.c \
      src/double_linked_list/lst_two_addback.c \
      src/double_linked_list/lst_two_size.c \
      src/double_linked_list/lst_two_delone.c \
      src/double_linked_list/lst_two_clear.c \
      src/double_linked_list/lst_two_iter.c \
      src/double_linked_list/lst_two_map.c \
	  \
      src/file/ft_random_filename.c
SRC_BONUS = src/linked_list/ft_lstadd_front.c \
	    src/linked_list/ft_lstnew.c \
	    src/linked_list/ft_lstsize.c \
	    src/linked_list/ft_lstadd_back.c \
	    src/linked_list/ft_lstlast.c \
	    src/linked_list/ft_lstdelone.c \
	    src/linked_list/ft_lstclear.c \
	    src/linked_list/ft_lstiter.c \
	    src/linked_list/ft_lstmap.c
DEPS = $(SRC:.c=.d)
DEPS_BONUS = $(SRC_BONUS:.c=.d)
OBJS = $(SRC:.c=.o)
OBJS_BONUS = $(SRC_BONUS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -MMD

all : $(NAME)

$(NAME) : $(OBJS)
	ar -rcs $@ $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus : $(OBJS_BONUS)
	ar -rcs $(NAME) $(OBJS_BONUS)
          
clean :
	rm -f $(OBJS) $(OBJS_BONUS)
	rm -f $(DEPS) $(DEPS_BONUS)

fclean : clean
	rm -f $(NAME)

re : fclean
	make all

-include $(DEPS)
-include $(DEPS_BONUS)

.PHONY :  all bonus clean fclean re