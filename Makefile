NAME = cub3D
CC = cc
SRCDIR = src
INCDIR = inc  
CFLAGS = -Wall -Wextra -O3 -g
IFLAGS := -I ./$(INCDIR) -I ./libft -L ./libft -lft -lm
MLXFLAGS = -framework Cocoa -framework OpenGL -framework IOKit -I./MLX42/include/MLX42 -L./MLX42/build -lmlx42 -lglfw

SOURCE_FILES = main.c \
			render.c \
			textures.c \
			movement.c \
			keys.c \
			reader/reader.c \
			reader/map_utils.c \
			reader/utils.c \
			reader/parsing.c \
			raycast/horizontal_intersection.c \
			raycast/vertical_intersection.c \
			raycast/raycast_utils.c \
			raycast/raycast.c
SRC = $(addprefix $(SRCDIR)/,$(SOURCE_FILES))

# libft
LIBFTDIR=libft
LIBFT=$(LIBFTDIR)/libft.a

all: $(LIBFT) make_mlx
	$(CC) $(CFLAGS) $(MLXFLAGS) $(IFLAGS) $(SRC) -o $(NAME)

# mlx
make_mlx:
	(cd MLX42 && cmake -B build && cmake --build build -j4)

$(LIBFT):
	make -C $(LIBFTDIR)

clean:
	make -C $(LIBFTDIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
