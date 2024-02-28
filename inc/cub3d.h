/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:35:05 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/28 12:04:27 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "../libft/libft.h"
#include "../MLX42/include/MLX42/MLX42.h"

#ifndef CUB3D_H
# define CUB3D_H

typedef struct s_vec {
	double	x;
	double	y;
	double	theta;
	double	triangle_angle;
}	t_vec;

typedef struct s_ray {
	int		win_x;
	int		wall_top;
	int		wall_bot;
	double	abs_ang;
	double	rel_ang;
	t_vec	cur_pos;
	t_vec	dir_vec;
	t_vec	step_vec;
	t_vec	x_step_vec;
	t_vec	y_step_vec;
}	t_ray;

typedef struct s_entity {
	double	x;
	double	y;
	double	theta;
	double	dx;
	double	dy;
}	t_entity;

typedef struct s_textures {
	char		*no_path;
	char		*so_path;
	char		*we_path;
	char		*ea_path;
	mlx_image_t	*no;
	mlx_image_t	*so;
	mlx_image_t	*we;
	mlx_image_t	*ea;
}	t_textures;

typedef struct s_texture_names {
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
}	t_texture_names;

typedef struct s_map {
	int	h;
	int	w;
	uint32_t	floor_col;
	uint32_t	sky_col;
	char	**grid;
	t_textures		textures;
}	t_map;

typedef struct s_rect {
	int			x;
	int			y;
	int			dx;
	int			dy;
	int			mapx;
	int			mapy;
	uint32_t	color;
}	t_rect;

typedef struct s_data {
	int			map_h;
	int			map_w;
	t_map		map;
	t_entity	player;
	t_entity	*entities;
	mlx_t		*mlx_win;
	mlx_image_t	*mlx_img;
	t_textures textures;
	t_texture_names texture_names;
	t_rect		rect;
	t_ray		ray;
}	t_data;

# define ER_FATAL -1
# define ER_MAP_NAME -2

int	parse_map(t_data *data, char *pathname);

int	print_mapp(t_data *data);

void	terminate(t_data *data, const char *err_message);

int		load_mlx_data(t_data *data);

int		ft_atoi_cub(char *s);

void	drawscreen(void *ptr);

#endif
