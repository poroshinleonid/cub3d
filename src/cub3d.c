/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:53:15 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/25 16:25:28 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include "../../libft/libft.h"
#include "../../MLX42/include/MLX42/MLX42.h"

int	main(int argc, char **argv)
{
	t_data	data;
	(void)argc;
	(void)argv;

	parse_map(&data, argv[1]);
	load_mlx_data(&data);
	print_mapp(&data);
	mlx_loop(data.mlx_win);
}
