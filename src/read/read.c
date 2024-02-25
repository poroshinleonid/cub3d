/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/25 18:15:52 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
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

int	load_map(t_data *data, int fd)
{
	int	i;

	while (save_map_meta(data, fd) == 0)
		(void)i;
	//Check that map is valid (player exists, there are walls, etc)
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
	close(fd);
	return (EXIT_SUCCESS);
}

