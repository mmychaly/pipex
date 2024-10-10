NAME = pipex
SRC_F = bonus/free_error_bonus.c \
				bonus/free_strs_fd_bonus.c \
				bonus/ft_envp_cherch_bonus.c \
				bonus/ft_launch_child_1_bonus.c \
				bonus/ft_launch_child_2_bonus.c \
				bonus/ft_launch_child_other_bonus.c \
				bonus/launch_here_doc_bonus.c \
				bonus/pipex_utils_bonus.c \
				bonus/pipex_bonus.c
CC = cc
FLAGS = -Wall -Werror -Wextra
RM = rm -f

$(NAME): $(SRC_F)
	@make all -C libft
	$(CC) $(FLAGS) $^ -o $@ -Llibft -lft -Ibonus

all: $(NAME)

clean: 
	make clean -C libft

fclean: clean
	make fclean -C libft
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re