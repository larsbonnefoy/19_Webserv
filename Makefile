# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: larsbonnefoy <larsbonnefoy@student.42.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/25 15:04:20 by larsbonnefo       #+#    #+#              #
#    Updated: 2023/04/25 15:04:21 by larsbonnefo      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
OBJS_DIR = Objs

FILES = main.cpp Config.cpp ConfigParser.cpp Server.cpp Location.cpp

SRCS = $(addprefix Sources/, $(FILES))

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

# ===---===---===---===---===---===---===---===---===---===---===---===---

CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g #-Wno-unused-variable -Wno-unused-parameter
CPPFLAGS = -std=c++98 
INCLUDES = -I Includes

# ===---===---===---===---===---===---===---===---===---===---===---===---

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/Sources

$(NAME): $(OBJS)
	c++ $(CFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: %.cpp
	c++ $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
