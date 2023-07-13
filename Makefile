# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hdelmas <hdelmas@student.s19.be>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/25 15:04:20 by larsbonnefo       #+#    #+#              #
#    Updated: 2023/07/13 13:28:03 by hdelmas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = webserv
OBJS_DIR = Objs

FILES = main.cpp Config.cpp ConfigParser.cpp Server.cpp Location.cpp Mux.cpp Log.cpp Socket.cpp Http.cpp HttpResponse.cpp HttpRequest.cpp  RequestHandler.cpp Cgi.cpp

SRCS = $(addprefix Sources/, $(FILES))

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))

# ===---===---===---===---===---===---===---===---===---===---===---===---

CFLAGS = -Wall -Wextra -Werror -g #-Wno-unused-variable -Wno-unused-parameter

CPPFLAGS = -std=c++98  -fsanitize=address

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
	rm -f webserv.log

re: fclean all

.PHONY: all clean fclean re
