# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/11 13:07:46 by adpachec          #+#    #+#              #
#    Updated: 2023/05/11 13:07:46 by adpachec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME 		=	philo

# Project directories
SRCDIR 		=	srcs
INCDIR 		=	include
OBJDIR 		=	obj

# Compiler and flags
CC 			=	gcc
CFLAGS 		=	-Wall -Wextra -Werror 
LDFLAGS 	= 	-pthread

# Para adpachec
INCLUDES 	=	-I$(INCDIR)
# LEAKS 		=	-fsanitize=address -g3

# Source files
SRCS		=	srcs/check_args.c \
				srcs/check_death.c \
				srcs/eat.c \
				srcs/free_structs.c \
				srcs/init_forks.c \
				srcs/init_philos.c \
				srcs/main.c \
				srcs/not_libft.c \
				srcs/sleep_think.c \
				srcs/start_eating.c \
				srcs/utils.c

# Object files
OBJS 		= 	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Color codes for terminal output
RED 		= 	\033[0;31m
GREEN	 	= 	\033[0;32m
YELLOW 		= 	\033[0;33m
RESET 		= 	\033[0m

# Delete files
RM 			= 	rm -rf

# Default target
all			: 	$(NAME)


$(OBJDIR)/%.o: 	$(SRCDIR)/%.c
				@mkdir -p $(dir $@)
				@printf "Compiling $(YELLOW)$<$(RESET)\r"
				@$(CC) $(CFLAGS) $(LEAKS) $(INCLUDES) -c $< -o $@
				@printf "                                                 \r"

# Link program
$(NAME)		: 	$(OBJS)
				@echo "Linking object files... \c"
				@$(CC) $(OBJS) $(CFLAGS) $(LEAKS) $(LDFLAGS) -o $(NAME)
				@echo "$(GREEN)$(NAME) created successfully !$(RESET)"

# Clean object files
clean		:
				@echo "$(RED)Deleting object files...$(RESET)\c"
				@$(RM) $(OBJDIR)
				@echo "$(GREEN) all object files DELETED !$(RESET)"

# Clean everything and recompile
fclean		: 	clean
				@echo "$(RED)Deleting object and binary files...$(RESET)\c"
				@$(RM) $(NAME)
				@echo "$(GREEN)all files DELETED !$(RESET)"

# Recompile everything
re			: 	fclean all

# Prevent errors if object files are deleted
-include $(OBJS:.o=.d)

# Phony targets
.PHONY		: 	all clean fclean re
