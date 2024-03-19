#ifndef CUB3D_H
# define CUB3D_H

# include "MLX42.h"

typedef struct s_map
{
	char		**grid;
	int32_t		floor_col;
	int32_t		sky_col;
	int32_t		w;
	int32_t		h;
}				t_map;

typedef struct s_player
{
	float		x;
	float		y;
	float		a;
	float		dx;
	float		dy;
}				t_player;

typedef struct s_textures
{
	char		*no_path;
	char		*so_path;
	char		*we_path;
	char		*ea_path;
	mlx_image_t	*no;
	mlx_image_t	*so;
	mlx_image_t	*we;
	mlx_image_t	*ea;
}				t_textures;

typedef struct s_data
{
	t_map		map;
	t_player	player;
	mlx_t		*mlx;
	mlx_image_t	*img_map;
	mlx_image_t	*img_3d;
	int32_t		img_width;
	int32_t		img_height;
	t_textures	textures;
}				t_data;

typedef struct s_point
{
	float		x;
	float		y;
	bool		exist;
}				t_point;

#endif
