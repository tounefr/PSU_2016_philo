
CC = gcc

RM = rm -f

SRCS = main.c

OBJS = $(SRCS:.c=.o)

NAME = philo 

LDFLAGS = -L. -lriceferee -lpthread

CFLAGS = -L. -lriceferee -lpthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
