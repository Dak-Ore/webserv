NAME = webserv

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[0;33m
RESET = \033[0m

FLAGS = -Wall -Wextra -Werror -g -std=c++98
INC = -I./includes
SRCDIR = src
OBJDIR = obj

SOURCES = ./Config.cpp ./ConfigParser.cpp ./EPoll.cpp ./EPollEvent.cpp ./File.cpp ./HttpMessage.cpp ./HttpRequest.cpp ./HttpResponse.cpp ./LocationConfig.cpp ./main.cpp ./ServerConfig.cpp ./Server.cpp ./Socket.cpp ./utils.cpp ./Webserv.cpp 

OBJS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)...$(RESET)"
	@c++ $(FLAGS) $(OBJS) -o $(NAME) $(INC)
	@echo "$(GREEN)Compilation finished successfully!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@c++ $(FLAGS) $(INC) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Cleaning all...$(RESET)"
	@rm -f $(NAME)

re: fclean sources $(NAME)

sources:
	@./update_sources

.PHONY: all clean fclean re sources