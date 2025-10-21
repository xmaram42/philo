NAME        = philo

SRC         = main.c utils.c init.c usleep.c routine.c clean.c
OBJ         = $(SRC:.c=.o)

CC          = cc
FLAGS       = -Wall -Wextra -Werror -I.
DEL         = rm -rf


GREEN = \033[0;32m
BLUE = \033[0;34m
PINK =  \033[38;5;213m
RESET = \033[0m

ART_FILE = art/pok.txt

all: $(NAME)

$(NAME): $(OBJ)
	@echo ""
	@echo "$(PINK)"
	@cat $(ART_FILE) 2>/dev/null || true
	@echo "$(RESET)"
	@echo "$(GREEN)Linking philo...$(RESET)"
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ)

%.o: %.c
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@echo "$(BLUE)Cleaning object files...$(RESET)"
	@$(DEL) $(OBJ)

fclean: clean
	@echo "$(BLUE)Cleaning executable...$(RESET)"
	@$(DEL) $(NAME)

re: fclean all

.PHONY: all clean fclean re
