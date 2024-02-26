#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "MLX42.h"
#define WIDTH 512
#define HEIGHT 512
#include "textures.h"
#include "cub3d.h"
#include "engine.h"

// static void error(void)
// {
// 	puts(mlx_strerror(mlx_errno));
// 	exit(EXIT_FAILURE);
// }

// int32_t	main(void)
// {
// 	// Start mlx
// 	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "Test", true);
// 	if (!mlx)
//         error();

// 	// Try to load the file
// 	t_data data[1];
// 	data->texture_names.no = "./textures/image.png";
// 	data->texture_names.we = "./textures/image.png";
// 	data->texture_names.ea = "./textures/image.png";
// 	data->texture_names.so = "./textures/image.png";
// 	data->mlx_win = mlx;
// 	data->player.x = 0;
// 	data->player.y = 0;
// 	data->player.theta = 0;
// 	data->player.dx = 0;
// 	data->player.dy = 0;
// 	load_textures(data);

// 	// Display the image
// 	mlx_image_t *img = mlx_new_image(mlx, 300, 300);
// 	for (int i = 0; i < 300; i++) {
// 		for (int j = 0; j < 300; j++) {
// 			mlx_put_pixel(img, i, j, put_pixel_color(data->textures.ea, (double) i / 300.0, (double) j / 300.0));
// 		}
// 	}

// 	if (mlx_image_to_window(mlx, img, 0, 0))
// 		error();

// 	mlx_loop_hook(data->mlx_win, ft_hook, data);
// 	mlx_loop(mlx);

// 	// Optional, terminate will clean up any leftovers, this is just to demonstrate.
// 	mlx_delete_image(mlx, img);
// 	mlx_terminate(mlx);
// 	return (EXIT_SUCCESS);
// }