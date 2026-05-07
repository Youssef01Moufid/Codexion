NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC_DIR = srcs
INC_DIR = includes

SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/parsing.c \
	$(SRC_DIR)/init.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/log.c \
	$(SRC_DIR)/scheduler.c \
	$(SRC_DIR)/dongle.c \
	$(SRC_DIR)/coder.c \
	$(SRC_DIR)/monitor.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -I$(INC_DIR) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all