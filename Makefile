NAME		= webserv
RM			= rm -rf
CXX			= g++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -pedantic-errors -g
INC_DIR		= ./includes
SRC_DIR		= ./src
OBJ_DIR		= ./obj

SRCS		= $(addprefix $(SRC_DIR)/, \
				main.cpp WebServer.cpp \
				0-config/ParserConfig.cpp \
				0-config/Utils.cpp \
				0-config/Request.cpp \
				0-config/Response.cpp \
				0-config/Poll.cpp \
				1-models/Autoindex.cpp \
				1-models/Server.cpp \
				1-models/ServerLocation.cpp \
				1-models/Socket.cpp \
				1-models/CgiHandler.cpp \
				1-models/TypeHelper.cpp \
			)

OBJS		= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

INC_DIRS	= $(shell find $(INC_DIR) -type d)
INC_FLAGS	= $(addprefix -I,$(INC_DIRS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
