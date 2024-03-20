/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trusanov <trusanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:27:58 by trusanov          #+#    #+#             */
/*   Updated: 2024/03/20 16:27:59 by trusanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_H
# define TEXTURES_H

# include "MLX42.h"
# include "cub3d.h"
# include <stdlib.h>

// load textures into data, quit if fail
void		load_textures(t_data *data);

// x, y in range 0..1
// x - width, y - height
uint32_t	put_pixel_color(mlx_image_t *texture, double x, double y);

mlx_image_t	*get_texture_type(bool north_dir, bool east_dir, t_point point,
				t_data *data);

uint32_t	get_texture_color(t_data *data, mlx_image_t *texture, t_point point,
				float text_y);

#endif
