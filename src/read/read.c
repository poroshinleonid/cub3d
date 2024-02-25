/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/25 15:27:07 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include "../../inc/cub3d.h"
#include "../../libft/libft.h"

int	load_map_line(t_data *data, char *line, int i)
{
	data->map.grid[i] = ft_strdup(line);
	return (0);
}

int	load_data_part(t_data *data,char **parts)
{
	printf("Loading data. [%s][%s]\n", parts[0], parts[1]);
	if (!ft_strcmp(parts[0], "NO"))
		data->map.textures.no_path = parts[1];
	else if (!ft_strcmp(parts[0], "SO"))
		data->map.textures.so_path = parts[1];
	else if (!ft_strcmp(parts[0], "EA"))
		data->map.textures.ea_path = parts[1];
	else if (!ft_strcmp(parts[0], "WE"))
		data->map.textures.we_path = parts[1];
	else if (!ft_strcmp(parts[0], "F"))
		data->map.floor_col = parts[1];
	else if (!ft_strcmp(parts[0], "C"))
		data->map.sky_col = parts[1];
	return (0);
}

int	load_map(t_data *data, int fd)
{
	char	*line;
	char	**split_line;
	int		i;

	line = ft_strtrim(get_next_line(fd), "\n");
	printf("read [%s]\n", line);
	write(1, "B", 1);
	split_line = ft_split(line, ' ');
	ft_printf("splitted %d\n", get_str_arr_sz(split_line));
	while (get_str_arr_sz(split_line) == 2)
	{
		write(1, "U", 1);
		load_data_part(data, split_line);
		line = ft_strtrim(get_next_line(fd), "\n");
		split_line = ft_split(line, ' ');
	}
	i = 0;
	data->map.grid = ft_calloc(100, sizeof(char *));
	while (get_str_arr_sz(split_line) == 1)
	{
		ft_printf("Split line[1]: /%s/\n", split_line[0]);
		ft_printf("Origi line[1]: /%s/\n", line);
		load_map_line(data, split_line[0], i);
		line = ft_strtrim(get_next_line(fd), "\n");
		split_line = ft_split(line, ' ');
		i++;
	}
	return (0);
}

int	parse_map(t_data *data, char *pathname)
{
	int	fd;

	printf("pathname [%s]\n", pathname);
	if (pathname == NULL || *pathname == '\0')
		return (EXIT_FAILURE);
	fd = open(pathname, O_RDONLY);
	if (fd < 0)
	{
		printf("fd error");
		exit(1);
	}
	load_map(data, fd);
	//load_textures(data, fd);
	close(fd);
	return (EXIT_SUCCESS);
}

