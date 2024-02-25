/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:04:19 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/25 15:16:53 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"

extern int	errno;

static void	print_error(const char *message)
{
	int	ft_errno;

	ft_errno = errno;
	write(STDERR_FILENO, "Error\n", 6);
	if (message == NULL)
	{
		if (ft_errno)
			perror("cub3d");
		else
			write(STDERR_FILENO, "cub3d: FATAL\n", 13);
		return ;
	}
	write(STDERR_FILENO, "cub3d: ", 7);
	ft_putendl_fd((char *)message, STDERR_FILENO);
}

void	terminate(t_data *data, const char *err_message)
{
	(void)data;
	print_error(err_message);
	//free_all();
	exit(1);
}