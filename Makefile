# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbeylot <sbeylot@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/14 11:16:25 by sbeylot           #+#    #+#              #
#    Updated: 2023/03/06 09:45:19 by sbeylot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			:=	c++
CXXFLAGS	:=	-Wall -Werror -Wextra -std=c++98 -g3
RM			:=	rm -rf


STACK_DIR	:=	srcs/stack_tests/
STACK_MAIN	:=	stack_tests.cpp
STACK_SRCS	:=	$(addprefix $(STACK_DIR), $(STACK_MAIN))

VECTOR_DIR	:=	srcs/vector_tests/
VECTOR_MAIN	:=	vector_tests.cpp
VECTOR_SRCS	:=	$(addprefix $(VECTOR_DIR), $(VECTOR_MAIN))

MAP_DIR		:=	srcs/map_tests/
MAP_MAIN	:=	map_tests.cpp
MAP_SRCS	:=	$(addprefix $(MAP_DIR), $(MAP_MAIN))


OBJS_DIR	:=	objs/
OBJS_DIR_FT	:=	$(OBJS_DIR)ft/
OBJS_DIR_STD:=	$(OBJS_DIR)std/

OBJS_NAME_S	:=	$(patsubst %.cpp, %.o, $(STACK_MAIN))
OBJS_NAME_V	:=	$(patsubst %.cpp, %.o, $(VECTOR_MAIN))
OBJS_NAME_M	:=	$(patsubst %.cpp, %.o, $(MAP_MAIN))

OBJS_FT_S	:=	$(addprefix $(OBJS_DIR_FT), $(OBJS_NAME_S))
OBJS_STD_S	:=	$(addprefix $(OBJS_DIR_STD), $(OBJS_NAME_S))
OBJS_FT_V	:=	$(addprefix $(OBJS_DIR_FT), $(OBJS_NAME_V))
OBJS_STD_V	:=	$(addprefix $(OBJS_DIR_STD), $(OBJS_NAME_V))
OBJS_FT_M	:=	$(addprefix $(OBJS_DIR_FT), $(OBJS_NAME_M))
OBJS_STD_M	:=	$(addprefix $(OBJS_DIR_STD), $(OBJS_NAME_M))


HEADER_DIR	:= /includes/
HEADER_NAME	:= vector.hpp\
			   stack.hpp\
			   map.hpp\
			   RBtree.hpp\
			   iterator_traits.hpp\
			   reverse_iterator.hpp\
			   utility.hpp

HEADER		:= -I.$(HEADER_DIR)

BIN_DIR			:=	bin/
BIN_DIR_STACK	:=	$(BIN_DIR)stack_tests/
BIN_DIR_VECTOR	:=	$(BIN_DIR)vector_tests/
BIN_DIR_MAP		:=	$(BIN_DIR)map_tests/

FT_STACK_NAME	:= $(BIN_DIR_STACK)ft_stack
STD_STACK_NAME	:= $(BIN_DIR_STACK)std_stack

FT_VECTOR_NAME	:= $(BIN_DIR_VECTOR)ft_vector
STD_VECTOR_NAME	:= $(BIN_DIR_VECTOR)std_vector

FT_MAP_NAME		:= $(BIN_DIR_MAP)ft_map
STD_MAP_NAME	:= $(BIN_DIR_MAP)std_map


DEFINE_STD	:=	-D STD

RED 		=	\033[0;31m
GREEN 		=	\033[0;32m
NONE 		=	\033[0m

all: $(BIN_DIR) $(FT_STACK_NAME) $(STD_STACK_NAME) $(FT_VECTOR_NAME) $(STD_VECTOR_NAME) $(FT_MAP_NAME) $(STD_MAP_NAME)

$(FT_STACK_NAME): stack
$(STD_STACK_NAME): stack
$(FT_VECTOR_NAME): vector 
$(STD_VECTOR_NAME): vector
$(FT_MAP_NAME): map 
$(STD_MAP_NAME): map

$(BIN_DIR):
	@echo "[ $(GREEN)✓$(NONE) ] Bin Folders created"
	@mkdir -p $(BIN_DIR_STACK)
	@mkdir -p $(BIN_DIR_VECTOR)
	@mkdir -p $(BIN_DIR_MAP)

stack: $(BIN_DIR) $(OBJS_FT_S) $(OBJS_STD_S)
	@$(CC) $(CXXFLAGS) $(HEADER) $(OBJS_FT_S) -o $(FT_STACK_NAME) 
	@$(CC) $(CXXFLAGS) $(HEADER) $(DEFINE_STD) $(OBJS_STD_S) -o $(STD_STACK_NAME) 
	@./$(FT_STACK_NAME) > $(FT_STACK_NAME)_output && ./$(STD_STACK_NAME) > $(STD_STACK_NAME)_output
	@echo "[ $(GREEN)✓$(NONE) ] stack binary created"
	@echo "[ $(GREEN)✓$(NONE) ] stack diff file created"
	@-diff $(FT_STACK_NAME)_output $(STD_STACK_NAME)_output > $(BIN_DIR_STACK)stack.diff || exit 0

vector: $(BIN_DIR) $(OBJS_FT_V) $(OBJS_STD_V)
	@$(CC) $(CXXFLAGS) $(HEADER) $(OBJS_FT_V) -o $(FT_VECTOR_NAME) 
	@$(CC) $(CXXFLAGS) $(HEADER) $(DEFINE_STD) $(OBJS_STD_V) -o $(STD_VECTOR_NAME) 
	@./$(FT_VECTOR_NAME) > $(FT_VECTOR_NAME)_output && ./$(STD_VECTOR_NAME) > $(STD_VECTOR_NAME)_output
	@echo "[ $(GREEN)✓$(NONE) ] vector binary created"
	@echo "[ $(GREEN)✓$(NONE) ] vector diff file created"
	@-diff $(FT_VECTOR_NAME)_output $(STD_VECTOR_NAME)_output > $(BIN_DIR_VECTOR)vector.diff || exit 0

map: $(BIN_DIR) $(OBJS_FT_M) $(OBJS_STD_M)
	@$(CC) $(CXXFLAGS) $(HEADER) $(OBJS_FT_M) -o $(FT_MAP_NAME) 
	@$(CC) $(CXXFLAGS) $(HEADER) $(DEFINE_STD) $(OBJS_STD_M) -o $(STD_MAP_NAME) 
	@./$(FT_MAP_NAME) > $(FT_MAP_NAME)_output && ./$(STD_MAP_NAME) > $(STD_MAP_NAME)_output
	@echo "[ $(GREEN)✓$(NONE) ] MAP binary created"
	@echo "[ $(GREEN)✓$(NONE) ] MAP diff file created"
	@-diff $(FT_MAP_NAME)_output $(STD_MAP_NAME)_output > $(BIN_DIR_MAP)map.diff || exit 0


$(OBJS_DIR_FT)%.o: $(STACK_DIR)%.cpp
	@mkdir -p $(OBJS_DIR_FT)
	@$(CC) $(CXXFLAGS) $(HEADER) -c $< -o $@
	@echo "[ $(GREEN)✓$(NONE) ] ft_stack object created"

$(OBJS_DIR_FT)%.o: $(VECTOR_DIR)%.cpp
	@mkdir -p $(OBJS_DIR_FT)
	@$(CC) $(CXXFLAGS) $(HEADER) -c $< -o $@
	@echo "[ $(GREEN)✓$(NONE) ] ft_vector object created"

$(OBJS_DIR_STD)%.o: $(STACK_DIR)%.cpp
	@mkdir -p $(OBJS_DIR_STD)
	@$(CC) $(CXXFLAGS) $(HEADER) $(DEFINE_STD) -c $< -o $@
	@echo "[ $(GREEN)✓$(NONE) ] std_vector object created"

$(OBJS_DIR_STD)%.o: $(VECTOR_DIR)%.cpp
	@mkdir -p $(OBJS_DIR_STD)
	@$(CC) $(CXXFLAGS) $(HEADER) $(DEFINE_STD) -c $< -o $@
	@echo "[ $(GREEN)✓$(NONE) ] ft_vector object created"

$(OBJS_DIR_STD)%.o: $(MAP_DIR)%.cpp
	@mkdir -p $(OBJS_DIR_STD)
	@$(CC) $(CXXFLAGS) $(HEADER) $(DEFINE_STD) -c $< -o $@
	@echo "[ $(GREEN)✓$(NONE) ] std_map object created"

$(OBJS_DIR_FT)%.o: $(MAP_DIR)%.cpp
	@mkdir -p $(OBJS_DIR_FT)
	@$(CC) $(CXXFLAGS) $(HEADER) -c $< -o $@
	@echo "[ $(GREEN)✓$(NONE) ] ft_map object created"


clean:
	@$(RM) $(OBJS_DIR)

fclean: clean
	@$(RM) $(BIN_DIR)
	@$(RM) $(NAME)
	@echo "[ $(GREEN)✓$(NONE) ] Project clean"

re: fclean all

.SECONDARY: $(OBJS)

.PHONY:	all re fclean clean


define TITLE
\033[92m
   _____ ____  _   _ _______       _____ _   _ ______ _____    _______ ______  _____ _______  
  / ____/ __ \| \ | |__   __|/\   |_   _| \ | |  ____|  __ \  |__   __|  ____|/ ____|__   __| 
 | |   | |  | |  \| |  | |  /  \    | | |  \| | |__  | |__) |    | |  | |__  | (___    | |    
 | |   | |  | | . ` |  | | / /\ \   | | | . ` |  __| |  _  /     | |  |  __|  \___ \   | |    
 | |___| |__| | |\  |  | |/ ____ \ _| |_| |\  | |____| | \ \     | |  | |____ ____) |  | |    
  \_____\____/|_| \_|  |_/_/    \_\_____|_| \_|______|_|  \_\    |_|  |______|_____/   |_|    
													
\033[0m

endef
export TITLE

title:
	clear
	@echo "$$TITLE"

-include $(DEPS)




