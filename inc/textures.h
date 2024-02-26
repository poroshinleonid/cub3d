#ifndef TEXTURES_H
# define TEXTURES_H

#include <stdlib.h>
#include "cub3d.h"

// load textures into data, quit if fail
void load_textures(t_data *data);

// x, y in range 0..1
// x - width, y - height
uint32_t    put_pixel_color(mlx_image_t *texture, double x, double y);


#endif
