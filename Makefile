NAME		=	ircserv

CXX			=	c++
CXX_FLAGS		=	-Wall -Wextra -Werror -std=c++98 -g3

SRCS_DIR	=	srcs/
OBJ_DIR		=	objects/
SRCS_NAMES	=	main.cpp parse.cpp
OBJS_NAMES	=	$(SRCS_NAMES:.cpp=.o)
SRCS		=	$(addprefix $(SRCS_DIR), $(SRCS_NAMES))
OBJS		=	$(addprefix $(OBJ_DIR), $(OBJS_NAMES))

HEADER		=	-Iincludes/

GREEN		=	\033[1;32m
RED			=	\033[1;31m
YELLOW		=	\033[1;33m
ORANGE		=	\033[38;5;214m
PURPLE		=	\033[1;35m
RESET		=	\033[0m

SUCCESS		=	[ ✔ ]
ERROR		=	[ ✗ ]
CLEAN		=	[ ♻ ]
REMOVE		=	[ 🗑 ]
REDO		=	[ 🗘 ]

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) $(OBJS) $(HEADER) -o $(NAME) || (echo "\n$(RED) ======================= $(ERROR) Linking failed ! =============================== $(RESET)\n"; exit 1)
	@echo "$(GREEN) ======================= $(SUCCESS) Executable created ! =========================== $(RESET)"

$(OBJ_DIR)%.o: $(SRCS_DIR)%.cpp | $(OBJ_DIR)
	@$(CC) $(C_FLAGS) -c $< -o $@ $(HEADER) || (echo "\n$(RED) ======================= $(ERROR) Compilation failed ! =========================== $(RESET)\n"; exit 1)
	@echo "$(GREEN) ======================= $(SUCCESS) Successful compilation ! ======================= $(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW) ======================= $(CLEAN) Successful binary & dependances cleaning ! ===== $(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(ORANGE) ======================= $(REMOVE) Excutable deleted ! ============================ $(RESET)"

re: fclean all
	@echo "$(PURPLE) ======================= $(REDO) Redo completed ! =============================== $(RESET)"

.PHONY: all clean fclean re