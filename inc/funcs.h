#ifndef FUNCS_H
# define FUNCS_H

# include "cub3d.h"
# include <stdbool.h>

int		parse_map(t_data *data, char *pathname);

void	render(void *void_data);
void	listen_keys(void *void_data);
void	resize_hook(int32_t width, int32_t height, void *void_data);
void	draw_rays(t_data *data);

float	distance(t_point a, t_point b);
float	floor_map(float x);
float	ceil_map(float x);

t_point	horizontal_intersection(t_data *data, float ra);
t_point	vertical_intersection(t_data *data, float ra);

void	movement_hook(t_data *data);

#endif
