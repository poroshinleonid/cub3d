NAME=cub3D

CC=cc

CFLAGS = -Wall -Wextra -Werror
MLXFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -I./MLX42/include -L./MLX42/build -lmlx42 -lglfw

CFILES = ./src/cub3d.c \
		./src/debug/debug.c \
		./src/memory/ft_new.c \
		./src/read/read.c \
		./src/utils/ui.c
DEBUGFILES = src/debug/debug.c


brewinstall:
	brew install glfw
	brew install cmake
getmlx:
	(cd MLX42 && cmake -B build && cmake --build build -j4)

libft:
	make -C libft

testleo: libft getmlx
	$(CC) $(CFLAGS) $(MLXFLAGS) $(CFILES) $(DEBUGFILES) -o bin/testleo.c

test: getmlx
	$(CC) $(CFLAGS) $(MLXFLAGS) taetest/*.c

