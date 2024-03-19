#include "MLX42.h"
#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include "libft.h"
#include <stdlib.h>

static void	clear_3d(t_data *data)
{
	ft_memset(data->img_3d->pixels, 255, data->img_3d->width
		* data->img_3d->height * sizeof(int32_t));
}

void	render(void *void_data)
{
	t_data	*data;

	data = (t_data *)void_data;
	clear_3d(data);
	clear_3d(data);
	draw_rays(data);
}
