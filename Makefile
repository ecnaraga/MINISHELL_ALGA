NAME = minishell
#NAME_BONUS = 
SRC_DIR = srcs
#SRC_DIR_BONUS = 
OBJ_DIR = objs
#OBJ_DIR_BONUS = 
SRCS = main.c \
	   ft_parse.c \
	   ft_parse_bis.c \
	   ft_parse_utils.c \
	   ft_echo.c \
	   ft_free_split_minish.c \
	   ft_split_minish_utils.c \
	   ft_split_minish.c \
	   ft_strlcpy_minish.c \
	   ft_structtablen.c 
#SRCS_BONUS = 
SRCS := $(SRCS:%=$(SRC_DIR)/%)
#SRCS_BONUS := $(SRCS_BONUS:%=$(SRC_DIR_BONUS)/%)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
#OBJS_BONUS := $(SRCS_BONUS:$(SRC_DIR_BONUS)/%.c=$(OBJ_DIR_BONUS)/%.o)
DEPS := $(OBJS:.o=.d)
#DEPS := $(OBJS_BONUS:.o=.d)
CC = cc
C_FLAGS = -Wall -Wextra -Werror  -MMD
LIB_FLAGS = -lreadline
INC_LIBFT = ./libft/libft.a
DIR_DUP     = mkdir -p $(@D)

all : $(NAME)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(DIR_DUP)
	$(CC) $(C_FLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	make -j -C ./libft
	$(CC) $(C_FLAGS) $(OBJS) $(INC_LIBFT) $(LIB_FLAGS) -o $@

bonus : $(NAME)

#$(OBJ_DIR_BONUS)/%.o:$(SRC_DIR_BONUS)/%.c
#	$(DIR_DUP)
#	$(CC) $(C_FLAGS) -c $< -o $@

#$(NAME_BONUS) : $(OBJS_BONUS)
#	make -j -C ./libft
#	$(CC) $(C_FLAGS) $(OBJS_BONUS) $(INC_LIBFT) -o $@

-include $(DEPS)

clean :
	make -j clean -C ./libft
	rm -f $(OBJS)
	rm -rf objs
	rm -f $(DEPS)

fclean : clean
	make -j fclean -C ./libft
	rm -f minishell

re : fclean
	make all

.PHONY : all clean fclean re