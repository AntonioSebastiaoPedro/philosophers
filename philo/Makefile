# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ansebast <ansebast@student.42luanda.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 14:00:18 by ansebast          #+#    #+#              #
#    Updated: 2024/10/21 18:27:11 by ansebast         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFLAGS = -Wall -Werror -Wextra -g

SRC = death_utils.c forks_utils.c libfunctions.c philosophers.c philos_utils.c validations.c status_utils.c
OBJS = $(SRC:.c=.o)

GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking the objects...$(RESET)"
	cc $(CFLAGS) $(OBJS) -o $(NAME) 
	@echo "$(GREEN)Executable $(NAME) created successfully!$(RESET)"

clean:
	@echo "$(RED)Removing all object files...$(RESET)"
	rm -f $(OBJS)

fclean: clean
	@echo "$(RED)Removing $(NAME) executable ...$(RESET)"
	rm -f $(NAME)

re: fclean all