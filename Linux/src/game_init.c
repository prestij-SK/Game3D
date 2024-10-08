#include "../header/the_game.h"

void	game_data_delete(t_GameData *data)
{
	if (!data)
		return ;
	minimap_delete(&data->minimap);
	image_delete(&data->minimap.origin_image, data->mlx);
	image_delete(&data->minimap.small_image, data->mlx);
	image_delete(&data->view.image, data->mlx);
	image_delete(&data->north_wall, data->mlx);
	image_delete(&data->south_wall, data->mlx);
	image_delete(&data->west_wall, data->mlx);
	image_delete(&data->east_wall, data->mlx);
	image_delete(&data->door_tex, data->mlx);
	if (data->mlx && data->mlx_window)
		mlx_destroy_window(data->mlx, data->mlx_window);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
}

void	game_nullify_pointers(t_GameData *data)
{
	if (!data)
		return ;
	data->mlx = NULL;
	data->mlx_window = NULL;
	data->tex_p = NULL;
}

static t_Point2D	player_location(char **map, t_Point2D block_count)
{
	t_Point2D	pos;
	int			i;
	int			j;

	pos.x = -1;
	pos.y = -1;
	if (!map)
		return (pos);
	i = 0;
	while (i < block_count.y)
	{
		j = 0;
		while (j < block_count.x)
		{
			if (map[i][j] == 'P')
			{
				pos.x = j;
				pos.y = i;
				return (pos);
			}
			++j;
		}
		++i;
	}
	return (pos);
}

t_StatusCode	game_mlx_init(t_GameData *data)
{
	if (!data)
		return (NULL_POINTER_ERROR);
	data->mlx = mlx_init();
	if (!data->mlx)
		return (MLX_ERROR);
	data->mlx_window = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
	if (!data->mlx_window)
		return (MLX_WINDOW_ERROR);
	return (SUCCESS_EXIT);
}

t_StatusCode	game_data_init(t_GameData *data, char **map, t_Point2D block_count)
{
	t_StatusCode	status;
	t_Point2D		pos;

	if (!data || !map)
		return (NULL_POINTER_ERROR);
	status = minimap_init(&data->minimap, data->mlx, map, block_count);
	if (status != SUCCESS_EXIT)
		return (status);
	pos = player_location(map, block_count);
	status = player_init(&data->player, &data->minimap, pos, data->minimap.block_size);
	if (status != SUCCESS_EXIT)
		return (status);
	status = fp_view_init(&data->view, data->mlx);
	if (status != SUCCESS_EXIT)
		return (status);
	status = doors_data_init(data->minimap.doors, &data->minimap.door_count, data->minimap.map, data->minimap.block_count);
	if (status != SUCCESS_EXIT)
		return (status);
	input_reset_all(&data->input);
	return (SUCCESS_EXIT);
}