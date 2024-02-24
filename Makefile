NAME=cub3D

CC=cc

CFLAGS = -Wall -Wextra -Werror
MLXFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -I./MLX42/include -L./MLX42/build -lmlx42 -lglfw




brewinstall:
	brew install glfw
	brew install cmake
getmlx:
	(cd MLX42 && cmake -B build && cmake --build build -j4)

test: getmlx
	$(CC) $(CFLAGS) $(MLXFLAGS) taetest/*.c

