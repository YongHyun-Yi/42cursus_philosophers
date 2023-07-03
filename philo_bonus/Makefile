# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yonghyle <yonghyle@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/02 12:29:39 by yonghyle          #+#    #+#              #
#    Updated: 2023/06/22 10:13:08 by yonghyle         ###   ########.fr        #
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

CFLAGS = -Wall -Wextra -Werror $(DEBUGFLAGS)

DEBUGFLAGS = -fsanitize=thread -g

INC = -I ./includes

RM = rm -rf

NAME = philo_bonus

#-------------------------------------------

BONUS_SRCS = $(addprefix srcs/, \
	ft_parse_philo_bonus.c  ft_putnbr_bonus.c       ft_strlen_bonus.c       state_functions_bonus.c \
	ft_atoi_bonus.c         ft_philosophers_bonus.c ft_strdup_bonus.c       init_philo_bonus.c \
	ft_itoa_bonus.c         ft_putchar_bonus.c      ft_strisnum_bonus.c     philo_routine_bonus.c \
)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

#-------------------------------------------

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
# -03 최적화 플래그를 사용하면 데이터 레이스가 발생하지 않는다...

$(NAME): $(BONUS_OBJS)
		@$(CC) $(CFLAGS) $^ -o $@
		@echo $(GREEN)"\n==========================================================\n"$(EOC)
		@echo $(YELLOW)"                   PHILOSOPHER BONUS DONE"$(EOC)
		@echo $(GREEN)"\n==========================================================\n"$(EOC)

bonus: $(NAME)

clean:
		@$(RM) ${BONUS_OBJS}
		@echo $(GREEN)"\n==========================================================\n"$(EOC)
		@echo $(YELLOW)"                     PHILOSOPHER CLEAN"
		@echo $(GREEN)"\n==========================================================\n"$(EOC)

fclean:
		@make --no-print-directory clean
		@$(RM) $(NAME)
		@echo $(GREEN)"\n==========================================================\n"$(EOC)
		@echo $(YELLOW)"                     PHILOSOPHER FCLEAN"
		@echo $(GREEN)"\n==========================================================\n"$(EOC)

re:
		@make fclean
		@make all