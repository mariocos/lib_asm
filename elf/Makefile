NAME = woody

SRC_PATH = ./srcs
OBJ_DIR = ./objs

SRCS =	get_header.c

OBJ = $(SRCS:$(SRC_PATH)/%.c=$(OBJ_DIR)/%.o)

CC = cc
CFLAGS = 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME) Hello_world ; cp based ./Hello_world

re: fclean all

.PHONY: all clean fclean re