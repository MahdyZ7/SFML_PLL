NAME = sfml

SRC = main.cpp keys.cpp

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98 # -fsanitize=address

OBJDIR = obj

LIBS = -lsfml-graphics -lsfml-window -lsfml-system
# HEADERS = wave.hpp

_OBJS = $(SRC:.cpp=.o)
OBJS = $(addprefix $(OBJDIR)/,$(_OBJS))

$(shell mkdir -p $(OBJDIR))
all : $(NAME)


$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

$(NAME) : $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)
	./$(NAME) &

replit: $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

clean:
	rm -f $(OBJS) $(DEP)

fclean: clean
	rm -f $(NAME)

re : fclean all


.PHONY: clean fclean all re


# NAME = ft_containers

# SRC = main.cpp

# CC = c++

# CFLAGS = -Wall -Werror -Wextra -std=c++98 -MMD -MP -Iinclude -fsanitize=address

# OBJDIR = obj
# DEPDIR = dep
# # OBJSUBDIRS = Objects

# _OBJS = $(SRC:.cpp=.o)
# OBJS = $(addprefix $(OBJDIR)/,$(_OBJS))

# DEP = $(SRC:%.cpp=$(DEPDIR)/%.d)
# all : $(NAME)

# -include $(DEP)

# $(OBJDIR)/%.o: %.cpp | $(OBJDIR)
# 	if [ ! -d $(OBJDIR)]; then \
# 		mkdir -p $(OBJDIR); \
# 	fi;
# 	for dir in $(OBJSUBDIRS); do \
# 		if [ ! -d $(OBJDIR)/$$dir ]; then \
# 			mkdir -p $(OBJDIR)/$$dir; \
# 		fi; \
# 	done
# 	$(CC) $(CFLAGS) -c $^ -o $@ -MF $(DEPDIR)/$*.d

# $(NAME) : $(OBJS)
# 	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

# $(OBJ):
# 	mkdir -p $@

# clean:
# 	rm -f $(OBJS) $(DEP)

# fclean: clean
# 	rm -f $(NAME)

# re : fclean all


# .PHONY: clean fclean all re