####################################################################################################
######################################### Program Name #############################################
####################################################################################################

NAME	= webserv

####################################################################################################
######################################### Compiler #################################################
####################################################################################################

CC		= g++
CFLAGS	= -Wall -Wextra -std=c++98 -g #-Werror

####################################################################################################
##################################### Include Folders ##############################################
####################################################################################################

INCLUDE =	$(addprefix -I ./includes, \
			/ \
			/0-Utilities \
			/1-Iterators \
			/3-map \
			)

####################################################################################################
######################################### Headers files ############################################
####################################################################################################

MAIN_HEADERS	= $(addprefix ./includes/, \
					ft_algorithm.hpp \
					ft_iterator_types.hpp \
					map.hpp \
					vector.hpp \
					)

UTILS_HEADERS	= $(addprefix ./includes/0-Utilities/, \
					teste.hpp \
					)


####################################################################################################
########################################## Source files ############################################
####################################################################################################

SRC		= $(addprefix ./src/, \
          	main.cpp \
          	$(PARSER_SRC) \
		  	)

PARSER_SRC	= $(addprefix 0-config/, \
				Parser.cpp \
				)

####################################################################################################
########################################## Objects files ###########################################
####################################################################################################

%_ft.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(<:%.cpp=%_ft.o)

all: $(NAME)

OBJ_FT	= $(SRC:%.cpp=%_ft.o)


$(NAME): $(OBJ_FT)
	$(CC) $(CFLAGS) $(OBJ_FT) -o $(NAME)


####################################################################################################
########################################## Default Rules ###########################################
####################################################################################################

clean:
	rm -rf $(OBJ_STD) $(OBJ_FT)

fclean: clean
	rm -rf $(NAME) $(NAME2)

re: fclean all

.PHONY: all clean fclean re

####################################################################################################
####################################################################################################
####################################################################################################
