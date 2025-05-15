NAME = webserv

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[0;33m
RESET = \033[0m

FLAGS = -Wall -Wextra -Werror -g -std=c++98
INC = -I./includes
SRCDIR = src
OBJDIR = obj

SOURCES = ./Adress.cpp ./CGI.cpp ./Config.cpp ./ConfigParser.cpp ./EPoll.cpp ./EPollEvent.cpp ./File.cpp ./HttpClient.cpp ./HttpMessage.cpp ./HttpRequest.cpp ./HttpResponse.cpp ./LocationConfig.cpp ./main.cpp ./ServerConfig.cpp ./Server.cpp ./SessionManager.cpp ./Socket.cpp ./utils.cpp ./Webserv.cpp 

OBJS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

_PROGRESS_BAR_EXISTS=false

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)...$(RESET)"
	@c++ $(FLAGS) $(OBJS) -o $(NAME) $(INC)
	@echo "$(GREEN)Compilation finished successfully!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@c++ $(FLAGS) $(INC) -c $< -o $@
	@if $(_PROGRESS_BAR_EXISTS); then echo -n ' '; fi  # fill progress bar

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Cleaning all...$(RESET)"
	@rm -f $(NAME)

re:
	@$(MAKE) _PROGRESS_BAR_EXISTS=true --no-print-directory fclean sources _init_progress_bar $(OBJS) _end_progress_bar $(NAME)

sources:
	@./update_sources

_init_progress_bar:
	@echo -ne '\e[0;44m'
	@for i in $$(seq $(words $(OBJS))); do echo -n ' '; done
	@echo -ne '\r\e[0;46m'

_end_progress_bar:
	@echo "\e[0m"

.PHONY: all clean fclean re sources _init_progress_bar _end_progress_bar
