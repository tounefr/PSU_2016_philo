##
## Makefile for  in /mnt/PSU_2016_philo
## 
## Made by Thomas HENON
## Login   <henon_t@epitech.net>
## 
## Started on  Fri Mar 17 10:27:32 2017 Thomas HENON
## Last update Fri Mar 17 10:27:32 2017 Thomas HENON
##

CC = gcc

RM = rm -f

SRCS = main.c \
        philo_init.c \
        philo_algo.c \
        util.c

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
