/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:35:05 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/25 16:25:43 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../MLX42/include/MLX42/MLX42.h"

#ifndef CUB3D_H
# define CUB3D_H

# define WIN_HEIGHT	1920
# define WIN_WIDTH	1080

typedef struct s_entity {
	float	x;
	float	y;
	float	theta;
}	t_entity;

typedef struct s_textures {
	//how are we going to store textures? 
	//some mlx format or array of ints
	//or array of chars idk
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
}	t_textures;

typedef struct s_map {
	int	h;
	int	w;
	char	*floor_col;
	char	*sky_col;
	char	**grid;
	t_textures		textures;
}	t_map;

typedef struct s_data {
	int		map_h;
	int		map_w;
	t_map	map;
	t_entity	player;
	t_entity	*entities;
	mlx_t	*mlx_win;
	mlx_image_t	*mlx_img;
}	t_data;

# define ER_FATAL -1
# define ER_MAP_NAME -2

int	parse_map(t_data *data, char *pathname);

int	print_mapp(t_data *data);

void	terminate(t_data *data, const char *err_message);

int		load_mlx_data(t_data *data);

#endif
