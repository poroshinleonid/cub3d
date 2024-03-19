/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/19 14:39:05 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "cub3d.h"
#include "config.h"
#include "libft.h"

int	ft_atoi_cub(char *s)
{
	int	res;

	if (!s || *s < '0' || *s > '9')
		return (-1);
	res = 0;
	while (*s >= '0' && *s <= '9')
	{
		res *= 10;
		res += *(s++) - '0';
		if (res > 255)
			return (-1);
	}
	if (*s != '\0')
		return (-1);
	return (res);
}

int	terminate(t_data *data, char *s)
{
	return (0);
}

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
	printf(" [%s] [%s]\n", split[0], split[1]);
	if (!ft_strcmp(split[0], "NO"))
		data->textures.no_path = split[1];
	else if (!ft_strcmp(split[0], "SO"))
		data->textures.so_path = split[1];
	else if (!ft_strcmp(split[0], "EA"))
		data->textures.ea_path = split[1];
	else if (!ft_strcmp(split[0], "WE"))
		data->textures.we_path = split[1];
	else if (!ft_strcmp(split[0], "F"))
		data->map.floor_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	else if (!ft_strcmp(split[0], "S"))
		data->map.sky_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	return (0);
}

int	save_map_grid(t_data *data, char *line, int fd)
{
	data->map.w = ft_strlen(line);
	data->map.h= 0;
	//FIX ME
	data->map.grid = ft_calloc(100, sizeof(char *));
	int i = 0;
	while (line && line[0] != '\0')
	{
		ft_printf("Saving [%s] to map\n", line);
		if (!ft_consists_of(line, MAP_CHARSET) || \
		ft_strlen(line) != data->map.w)
		{
			free(line);
			return (-1);
		}
		data->map.grid[i++] = line;
		data->map.h += 1;
		line = ft_strtrim(get_next_line(fd), " \n");
	}
	return (1);
}


int	save_map_meta(t_data *data, int fd)
{
	char	*line;
	line = ft_strtrim(ft_strtrim(get_next_line(fd), "\n"), " ");
	printf("LIne: [%s]\n", line);
	if (line == NULL)
		return (-1);
	else if (ft_isdigit(line[0]))
		return (save_map_grid(data, line, fd));
	else if ((line[0] =='N' && data->textures.no_path == NULL) || \
		(line[0] =='S' && data->textures.so_path == NULL) || \
		(line[0] =='W' && data->textures.we_path == NULL) || \
		(line[0] =='E' && data->textures.ea_path == NULL) || \
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
	data->player.x = (x + 0.5) * MAP_S;
	data->player.y = (y + 0.5) * MAP_S;
	if (data->map.grid[y][x] == 'N')
		data->player.a = M_PI/2.0;
	else if (data->map.grid[y][x] == 'S')
		data->player.a = M_PI + M_PI/2.0;
	else if (data->map.grid[y][x] == 'W')
		data->player.a = M_PI;
	else if (data->map.grid[y][x] == 'E')
		data->player.a = 0.0;
	else
		return (1);
	data->map.grid[y][x] = 0;
	return (0);
}

int	find_player_pos(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->map.h)
	{
		j = 0;
		while (j < data->map.w)
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
	int	i;
	int	j;
	int	players_count;

	i = 0;
	j = 0;
	players_count = 0;
	while (i < data->map.h)
	{
		j = 0;
		while (j < data->map.w)
		{
			if (((j == 0 || j == data->map.w - 1) || \
				(i == 0 || i == data->map.h - 1)) && \
					data->map.grid[i][j] != '1')
				return (0);
			if (ft_in(data->map.grid[i][j], MAP_PLAYER_CHARSET))
				players_count++;
			j++;
		}
		i++;
	}
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
		terminate(data, "Map invalid\n");
	return (0);
}

int	parse_map(t_data *data, char *pathname)
{
	int	fd;

	data->map.floor_col = 0xFF000000;
	data->map.sky_col = 0xFF000000;
	data->textures.no_path = NULL;
	data->textures.so_path = NULL;
	data->textures.we_path = NULL;
	data->textures.ea_path = NULL;
	if (pathname == NULL || *pathname == '\0')
		return (EXIT_FAILURE);
	fd = open(pathname, O_RDONLY);
	if (fd < 0)
		terminate(data, "Can't parse the map\n");
	if (load_map(data, fd) != 0)
		terminate(data, "Can't load the map\n");
	// if (load_textures(data, fd) != 0)
	// 	terminate(data, "Can't load the textures\n");
	return (EXIT_SUCCESS);
}
