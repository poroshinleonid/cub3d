/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 16:55:59 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "cub3d.h"
#include "funcs.h"
#include "config.h"
#include "libft.h"

int	load_map(t_data *data, int fd)
{
	int		tmp;

	while (1)
	{
		tmp = save_map_metainf(data, fd);
		if (tmp == -1)
			terminate(data, "Can't read the map file");
		else if (tmp == 1)
			break ;
	}
	if (data->map.floor_col == 0 || data->map.sky_col == 0)
		terminate(data, "Can't read the colors");
	close(fd);
	if (data->map.h == -1)
		terminate(data, "Can't read the map file");
	normalize_map(data);
	find_player_pos(data);
	if (!is_map_valid(data))
		terminate(data, "Invalid map");
	replace_spaces(data);
	return (0);
}

void	prepare_data(t_data *data)
{
	data->map.h = -1;
	data->map.floor_col = 0x00000000;
	data->map.sky_col = 0x00000000;
	data->textures.no_path = NULL;
	data->textures.so_path = NULL;
	data->textures.we_path = NULL;
	data->textures.ea_path = NULL;
	data->mlx = NULL;
	data->img_map = NULL;
	data->img_3d = NULL;
	data->map.grid = NULL;
	data->textures.ea_path = NULL;
	data->textures.so_path = NULL;
	data->textures.we_path = NULL;
	data->textures.no_path = NULL;
	data->textures.ea = NULL;
	data->textures.so = NULL;
	data->textures.we = NULL;
	data->textures.no = NULL;
}

int	parse_map(t_data *data, char *pathname)
{
	int	fd;

	if (pathname == NULL || *pathname == '\0')
		return (EXIT_FAILURE);
	prepare_data(data);
	fd = open(pathname, O_RDONLY);
	if (fd < 0)
		terminate(data, "Can't open the map file");
	if (load_map(data, fd) != 0)
		terminate(data, "Can't read the map file");
	return (EXIT_SUCCESS);
}
