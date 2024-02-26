NAME = cub3D
CC = cc
SRCDIR = src
INCDIR = inc  
IFLAGS = -I ./inc -I ./libft
CFLAGS = -Wall -Wextra -Werror -g
MLXFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -I./MLX42/include/MLX42 -L./MLX42/build -lmlx42 -lglfw

SOURCE_FILES = cub3d.c \
		read/read.c \
		utils/ui.c \
		draw/draw.c \
		utils/misc.c \
		engine/movement.c
CFILES = $(addprefix $(SRCDIR)/,$(SOURCE_FILES))
MAINSOURCE := $(SRCDIR)/cub3d.c
DEBUGFILES = src/debug/debug.c \
			taetest/main.c

INCLUDE_FILES := \
					cub3d.h \
					config.h
INCLUDES := $(addprefix $(INCDIR)/,$(INCLUDE_FILES))
INC_FLAGS := -I ./$(INCDIR) -I ./libft -L ./libft -lft -lm

# libft
LIBFTDIR=libft
LIBFT=$(LIBFTDIR)/libft.a

# mlx
install: install_brew install_glfw

install_brew:
	rm -r $HOME/.brew && rm -rf $HOME/goinfre/.brew && \
	git clone --depth=1 https://github.com/Homebrew/brew $HOME/goinfre/.brew && \
	echo 'export PATH=$HOME/goinfre/.brew/bin:$PATH' >> $HOME/.zshrc && \
	source $HOME/.zshrc && brew update \

install_libraries:
	brew install glfw
	brew install cmake

install_mlx:
	(cd MLX42 && cmake -B build && cmake --build build -j4)

testleo: $(LIBFT) install_mlx
	$(CC) $(CFLAGS) $(MLXFLAGS) $(CFILES) $(DEBUGFILES) $(IFLAGS) -Llibft -lft  -o bin/testleo.out

taetexturetest: install_mlx
	$(CC) $(CFLAGS) $(MLXFLAGS) taetest/textures.c src/textures.c src/utils/ui.c -L./libft -lft $(IFLAGS)

taetest: install_mlx
	$(CC) $(CFLAGS) $(MLXFLAGS) taetest/main.c src/engine/movement.c src/textures.c src/utils/ui.c -L./libft -lft $(IFLAGS)

all: $(NAME)

$(NAME): $(SOURCES) $(MAINSOURCE) $(INCLUDES) $(LIBFT)
	$(CC) $(CFLAGS) $(INC_FLAGS) $(SOURCES) $(MAINSOURCE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	make -C $(LIBFTDIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all



.PHONY: all clean fclean re