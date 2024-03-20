/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:30:51 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 15:32:27 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "cub3d.h"
#include "config.h"
#include "libft.h"

char	*ft_str_extend(char *s, char filler, int reslen)
{
	char	*newstr;
	int		i;

	i = 0;
	newstr = ft_calloc(reslen + 1, sizeof(char));
	while (s[i])
	{
		newstr[i] = s[i];
		i++;
	}
	while (i < reslen)
	{
		newstr[i] = filler;
		i++;
	}
	free(s);
	return (newstr);
}

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

void	print_error(const char *message)
{
	write(STDERR_FILENO, "Error\n", 6);
	if (message == NULL)
	{
		write(STDERR_FILENO, "cub3d: FATAL ERROR\n", 13);
		return ;
	}
	write(STDERR_FILENO, "cub3d: ", 7);
	ft_putendl_fd((char *)message, STDERR_FILENO);
}

void	free_mem(t_data *data)
{
	if (data->textures.ea_path)
		free(data->textures.ea_path);
	if (data->textures.so_path)
		free(data->textures.so_path);
	if (data->textures.no_path)
		free(data->textures.no_path);
	if (data->textures.we_path)
		free(data->textures.we_path);
	if (!data->mlx && data->textures.ea)
		free(data->textures.ea);
	if (!data->mlx && data->textures.so)
		free(data->textures.so);
	if (!data->mlx && data->textures.no)
		free(data->textures.no);
	if (!data->mlx && data->textures.we)
		free(data->textures.we);
	if (data->map.grid)
		free_str_arr(&(data->map.grid));
	if (data->mlx)
		mlx_terminate(data->mlx);
}

void	terminate(t_data *data, const char *err_message)
{
	(void)data;
	print_error(err_message);
	free_mem(data);
	exit(1);
}
