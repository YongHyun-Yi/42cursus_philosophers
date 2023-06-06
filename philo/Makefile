# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/02 12:29:39 by yonghyle          #+#    #+#              #
#    Updated: 2023/06/02 12:38:12 by yonghyle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#-------------------------------------------

BLACK       =   "\033[0;30m"
GRAY        =   "\033[1;30m"
RED         =   "\033[1;31m"
GREEN       =   "\033[0;32m"
YELLOW      =   "\033[1;33m"
PURPLE      =   "\033[0;35m"
CYAN        =   "\033[1;36m"
WHITE       =   "\033[1;37m"
EOC         =   "\033[0;0m"
LINE_CLEAR  =   "\x1b[1A\x1b[M"

#-------------------------------------------

CC = cc

# CFLAGS = -Wall -Wextra -Werror

INC = -I ./includes

RM = rm -rf

NAME = philo

BONUS_NAME = make_bonus

#-------------------------------------------

# MANDATORY_SRCS = $(addprefix srcs/, \
)
MANDATORY_SRCS = $(wildcard srcs/*.c)
MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)

#-------------------------------------------

# BONUS_SRCS = $(addprefix bonus_srcs/,\
)
BONUS_SRCS = $(wildcard bonus_srcs/*.c)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

#-------------------------------------------

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -O3 -c $< -o $@

$(NAME): $(MANDATORY_OBJS)
		@$(CC) $(CFLAGS) $^ -g -o $@
		@echo $(GREEN)"\n==========================================================\n"$(EOC)
		@echo $(YELLOW)"                     PHILOSOPHER DONE"$(EOC)
		@echo $(GREEN)"\n==========================================================\n"$(EOC)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
		@$(CC) $(CFLAGS) $^ -g -o $(NAME)
		@touch $@
		@echo $(GREEN)"\n==========================================================\n"$(EOC)
		@echo $(YELLOW)"                  PHILOSOPHER BONUS DONE"$(EOC)
		@echo $(GREEN)"\n==========================================================\n"$(EOC)

clean:
		@$(RM) ${MANDATORY_OBJS} ${BONUS_OBJS}
		@echo $(GREEN)"\n==========================================================\n"$(EOC)
		@echo $(YELLOW)"                     PHILOSOPHER CLEAN"
		@echo $(GREEN)"\n==========================================================\n"$(EOC)

fclean:
		@$(RM) $(NAME) $(BONUS_NAME)
		@echo $(GREEN)"\n==========================================================\n"$(EOC)
		@echo $(YELLOW)"                     PHILOSOPHER FCLEAN"
		@echo $(GREEN)"\n==========================================================\n"$(EOC)

re:
		@make fclean
		@make all