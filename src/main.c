#include "MLX42.h"
#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include "textures.h"
#include <stdlib.h>

char	**make_grid(void)
{
	char	**grid;

	grid = malloc(H_MAP * sizeof(char *));
	for (int i = 0; i < W_MAP; i++)
	{
		grid[i] = malloc(H_MAP * sizeof(char));
	}
	grid[0][0] = '1';
	grid[0][1] = '1';
	grid[0][2] = '1';
	grid[0][3] = '1';
	grid[0][4] = '1';
	grid[0][5] = '1';
	grid[0][6] = '1';
	grid[0][7] = '1';
	grid[1][0] = '1';
	grid[1][1] = '0';
	grid[1][2] = '0';
	grid[1][3] = '1';
	grid[1][4] = '0';
	grid[1][5] = '0';
	grid[1][6] = '0';
	grid[1][7] = '1';
	grid[2][0] = '1';
	grid[2][1] = '0';
	grid[2][2] = '0';
	grid[2][3] = '0';
	grid[2][4] = '1';
	grid[2][5] = '0';
	grid[2][6] = '0';
	grid[2][7] = '1';
	grid[3][0] = '1';
	grid[3][1] = '0';
	grid[3][2] = '0';
	grid[3][3] = '0';
	grid[3][4] = '1';
	grid[3][5] = '0';
	grid[3][6] = '0';
	grid[3][7] = '1';
	grid[4][0] = '1';
	grid[4][1] = '0';
	grid[4][2] = '0';
	grid[4][3] = '0';
	grid[4][4] = '0';
	grid[4][5] = '0';
	grid[4][6] = '1';
	grid[4][7] = '1';
	grid[5][0] = '1';
	grid[5][1] = '1';
	grid[5][2] = '1';
	grid[5][3] = '1';
	grid[5][4] = '1';
	grid[5][5] = '1';
	grid[5][6] = '1';
	grid[5][7] = '1';
	return (grid);
}

void	load_stuff(t_data *data)
{
	//data->player.x = 125.0;
	//data->player.y = 75.0;
	//data->player.a = PI / 2;
	//data->map.grid = make_grid();
	//data->map.w = W_MAP;
	//data->map.h = H_MAP;
	data->mlx = mlx_init(INIT_WIN_WIDTH, INIT_WIN_HEIGHT, "cub3d", 1);
	data->img_map = mlx_new_image(data->mlx, INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	data->img_3d = mlx_new_image(data->mlx, INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	data->img_width = INIT_WIN_WIDTH;
	data->img_height = INIT_WIN_HEIGHT;
	// data->textures.no_path = "./textures/n.png";
	// data->textures.so_path = "./textures/s.png";
	// data->textures.ea_path = "./textures/a.png";
	// data->textures.we_path = "./textures/w.png";
	load_textures(data);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2 || parse_map(&data, argv[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	load_stuff(&data);
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop_hook(data.mlx, listen_keys, &data);
	mlx_resize_hook(data.mlx, resize_hook, &data);
	mlx_image_to_window(data.mlx, data.img_3d, 0, 0);
	mlx_loop(data.mlx);
}
