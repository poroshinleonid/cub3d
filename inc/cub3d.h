/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:35:05 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/24 18:13:20 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

typedef struct s_entity {
	float	x;
	float	y;
	float	theta;
}	t_entity;

typedef struct s_textures {
	//how are we going to store textures? 
	//some mlx format or array of ints
	//or array of chars idk
	int	no;
	int	so;
	int	we;
	int	ea;
}	t_textures;

typedef struct s_map {
	int	h;
	int	w;
	int32_t	floor_col;
	int32_t	sky_col;
	unsigned char	**grid;
	t_textures		textures;
}	t_map;

typedef struct s_data {
	int		map_h;
	int		map_w;
	t_map	map;
	t_entity	player;
	t_entity	*entities;
}	t_data;

# define ER_FATAL -1
# define ER_MAP_NAME -2

#endif