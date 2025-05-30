NAME = webserv

GREEN = \033[32m
RED = \033[31m
YELLOW = \033[0;33m
RESET = \033[0m

FLAGS = -Wall -Wextra -Werror -g -std=c++98
INC = -I./includes
SRCDIR = src
OBJDIR = obj

SOURCES = ./Adress.cpp ./CGI.cpp ./Config.cpp ./ConfigParser.cpp ./EPoll.cpp ./EPollEvent.cpp ./File.cpp ./HttpClient.cpp ./HttpMessage.cpp ./HttpRequest.cpp ./HttpResponse.cpp ./LocationConfig.cpp ./main.cpp ./ServerConfig.cpp ./Server.cpp ./ServerSocket.cpp ./Socket.cpp ./UploadHandler.cpp ./utils.cpp ./Webserv.cpp 

OBJS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

all: $(NAME)

$(NAME): _init_progress_bar $(OBJS) _end_progress_bar
	@echo "$(GREEN)Compiling $(NAME)...$(RESET)"
	@c++ $(FLAGS) $(OBJS) -o $(NAME) $(INC)
	@echo "$(GREEN)Compilation finished successfully!$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	@c++ $(FLAGS) $(INC) -c $< -o $@
	@echo -n ' '  # fill progress bar

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "$(RED)Cleaning all...$(RESET)"
	@rm -f $(NAME)

re: fclean sources $(NAME)

sources:
	@./update_sources

_init_progress_bar:
	@{ while kill -0 $$PPID 2> /dev/null; do true; done; echo -n '\e[?25h\e[0m'; }&
	@echo -ne '\e[?25l\e[0;44m'
	@for obj in $(OBJS); do \
		if ! $(MAKE) -q $$obj; then \
			echo -n ' ' \
		; fi \
	; done
	@echo -ne '\r\e[0;46m'

_end_progress_bar:
	@echo "\e[0m\e[?25h"

.PHONY: all clean fclean re sources _init_progress_bar _end_progress_bar
