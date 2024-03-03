/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/03 17:14:56 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "../../inc/cub3d.h"
#include "../../libft/libft.h"

int32_t	parse_color(t_data *data, char *line, int fd)
{
	int	r;
	int	g;
	int	b;
	char	**colors;
	colors = ft_split(line, ' ');
	if (colors == NULL)
	{
		free(line);
		close(fd);
		terminate(data, "Can't parse colors!\n");
	}
	r = ft_atoi_cub(colors[1]);
	g = ft_atoi_cub(colors[2]);
	b = ft_atoi_cub(colors[3]);
	if (r == -1 || g == -1 || b == -1)
	{
		free(line);
		close(fd);
		terminate(data, "Can't parse colors!!!\n");
	}
	return ((r<<16) + (g<<8) + (b));
}

int	save_map_map_info(t_data *data, char *line, int fd)
{
	char	**split;

	split = ft_split(line, ' ');
	if (get_str_arr_sz(split) != 2)
		return (-1);
	if (!ft_strcmp(split[0], "NO"))
		data->map.textures.no_path = split[1];
	else if (!ft_strcmp(split[0], "SO"))
		data->map.textures.so_path = split[1];
	else if (!ft_strcmp(split[0], "EA"))
		data->map.textures.ea_path = split[1];
	else if (!ft_strcmp(split[0], "WE"))
		data->map.textures.we_path = split[1];
	else if (!ft_strcmp(split[0], "F"))
		data->map.floor_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	else if (!ft_strcmp(split[0], "S"))
		data->map.sky_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	return (0);
}

int	save_map_grid(t_data *data, char *line, int fd)
{
	data->map_w = ft_strlen(line);
	data->map_h = 0;
	//FIX ME
	data->map.grid = ft_calloc(100, sizeof(char *));
	int i = 0;
	while (line && line[0] != '\0')
	{
		ft_printf("Saving [%s] to map\n", line);
		if (!ft_consists_of(line, MAP_CHARSET) || \
		ft_strlen(line) != data->map_w)
		{
			free(line);
			return (-1);
		}
		data->map.grid[i++] = line;
		data->map_h += 1;
		line = ft_strtrim(get_next_line(fd), " \n");
	}
	return (1);
}


int	save_map_meta(t_data *data, int fd)
{
	char	*line;
	line = ft_strtrim(ft_strtrim(get_next_line(fd), "\n"), " ");
	if (line == NULL)
		return (-1);
	else if (ft_isdigit(line[0]))
		return (save_map_grid(data, line, fd));
	else if ((line[0] =='N' && data->map.textures.no_path == NULL) || \
		(line[0] =='S' && data->map.textures.so_path == NULL) || \
		(line[0] =='W' && data->map.textures.we_path == NULL) || \
		(line[0] =='E' && data->map.textures.ea_path == NULL) || \
		(line[0] =='F' && data->map.floor_col == 0xFF000000) || \
		(line[0] =='C' && data->map.sky_col == 0xFF000000))
		return (save_map_map_info(data, line, fd));
	else if (line[0] == '\n')
		return (0);
	else
	{
		free(line);
		close(fd);
		terminate(data, "Can't read the map!!!\n");
	}
	return (0);
}

int	save_player_pos(t_data *data, int x, int y)
{
	data->player.x = x + 0.5;
	data->player.y = y + 0.5;
	if (data->map.grid[y][x] == 'N')
		data->player.theta = M_PI/2.0;
	else if (data->map.grid[y][x] == 'S')
		data->player.theta = M_PI + M_PI/2.0;
	else if (data->map.grid[y][x] == 'W')
		data->player.theta = M_PI;
	else if (data->map.grid[y][x] == 'E')
		data->player.theta = 0.0;
	else
		return (1);
	return (0);
}

int	find_player_pos(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->map_h)
	{
		j = 0;
		while (j < data->map_w)
		{
			if (!ft_isdigit(data->map.grid[i][j]))
			{
				save_player_pos(data, j, i);
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	is_map_valid(t_data *data)
{
	if (data->map.grid[0][0] != 1)
		return (0);
	return (1);
}

int	load_map(t_data *data, int fd)
{
	int	i;

	while (save_map_meta(data, fd) == 0)
		(void)i;
	close(fd);
	find_player_pos(data);
	if (!is_map_valid(data))
		terminate(data, "Map error\n");
	return (0);
}

int	parse_map(t_data *data, char *pathname)
{
	int	fd;

	if (pathname == NULL || *pathname == '\0')
		return (EXIT_FAILURE);
	fd = open(pathname, O_RDONLY);
	if (fd < 0)
		terminate(data, "Can't parse the map\n");
	if (load_map(data, fd) != 0)
		terminate(data, "Can't parse the map\n");
	// if (load_textures(data, fd) != 0)
	// 	terminate(data, "Can't parse the map\n");
	return (EXIT_SUCCESS);
}

