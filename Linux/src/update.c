#include "../header/the_game.h"

static int	is_in_minimap_range(t_Minimap *minimap, int y, int x)
{
	if ((y >= 0 && y < minimap->block_count.y) &&
		(x >= 0 && x < minimap->block_count.x))
	{
		return (B_TRUE);
	}
	return (B_FALSE);
}

static void	set_xy_offset(t_UpdateUtil *util, double change_x, double change_y)
{
	if (change_x < 0)
		util->offset.x = -COLLISION_GAP;
	else
		util->offset.x = COLLISION_GAP;
	if (change_y < 0)
		util->offset.y = -COLLISION_GAP;
	else
		util->offset.y = COLLISION_GAP;
}

// strafing left
void	update_A(t_Player *player)
{
	t_UpdateUtil	util;

    // Calculate the strafe vector (perpendicular to the delta vector)
    double strafe_angle = player->angle - M_PI / 2;  // 90 degrees rotation
	// Calculate delta X,Y again
    double strafe_x = cos(strafe_angle) * PLAYER_MOVE_SPEED;
    double strafe_y = sin(strafe_angle) * PLAYER_MOVE_SPEED;

    set_xy_offset(&util, strafe_x, strafe_y);
	util.current.x = player->pos.x / player->minimap->block_size;
	util.current.y = player->pos.y / player->minimap->block_size;
	util.next.x = (player->pos.x + util.offset.x) / player->minimap->block_size;
	util.next.y = (player->pos.y + util.offset.y) / player->minimap->block_size;
	if (is_in_minimap_range(player->minimap, util.current.y, util.next.x))
		if (player->minimap->map[util.current.y][util.next.x] != '1')
			player->pos.x += (int)round(strafe_x);
	if (is_in_minimap_range(player->minimap, util.next.y, util.current.x ))
		if (player->minimap->map[util.next.y][util.current.x ] != '1')
			player->pos.y += (int)round(strafe_y);
}

// strafing right
void	update_D(t_Player *player)
{
	t_UpdateUtil	util;

    // Same thing, but we add 90 degree
    double strafe_angle = player->angle + M_PI / 2;
	// Calculate delta X,Y again
    double strafe_x = cos(strafe_angle) * PLAYER_MOVE_SPEED;
    double strafe_y = sin(strafe_angle) * PLAYER_MOVE_SPEED;

	set_xy_offset(&util, strafe_x, strafe_y);
	util.current.x = player->pos.x / player->minimap->block_size;
	util.current.y = player->pos.y / player->minimap->block_size;
	util.next.x = (player->pos.x + util.offset.x) / player->minimap->block_size;
	util.next.y = (player->pos.y + util.offset.y) / player->minimap->block_size;
	if (is_in_minimap_range(player->minimap, util.current.y, util.next.x))
		if (player->minimap->map[util.current.y][util.next.x] != '1')
			player->pos.x += (int)round(strafe_x);
	if (is_in_minimap_range(player->minimap, util.next.y, util.current.x ))
		if (player->minimap->map[util.next.y][util.current.x ] != '1')
			player->pos.y += (int)round(strafe_y);
}

// going forward
void	update_W(t_Player *player)
{
	t_UpdateUtil	util;

	set_xy_offset(&util, player->delta.x, player->delta.y);
	util.current.x = player->pos.x / player->minimap->block_size;
	util.current.y = player->pos.y / player->minimap->block_size;
	util.next.x = (player->pos.x + util.offset.x) / player->minimap->block_size;
	util.next.y = (player->pos.y + util.offset.y) / player->minimap->block_size;
	if (is_in_minimap_range(player->minimap, util.current.y, util.next.x))
		if (player->minimap->map[util.current.y][util.next.x] != '1')
			player->pos.x += (int)round(player->delta.x);
	if (is_in_minimap_range(player->minimap, util.next.y, util.current.x ))
		if (player->minimap->map[util.next.y][util.current.x ] != '1')
			player->pos.y += (int)round(player->delta.y);
}

// going back
void	update_S(t_Player *player)
{
	t_UpdateUtil	util;

	set_xy_offset(&util, player->delta.x, player->delta.y);
	util.current.x = player->pos.x / player->minimap->block_size;
	util.current.y = player->pos.y / player->minimap->block_size;
	util.next.x = (player->pos.x - util.offset.x) / player->minimap->block_size;
	util.next.y = (player->pos.y - util.offset.y) / player->minimap->block_size;
	if (is_in_minimap_range(player->minimap, util.current.y, util.next.x))
		if (player->minimap->map[util.current.y][util.next.x] != '1')
			player->pos.x -= (int)round(player->delta.x);
	if (is_in_minimap_range(player->minimap, util.next.y, util.current.x ))
		if (player->minimap->map[util.next.y][util.current.x ] != '1')
			player->pos.y -= (int)round(player->delta.y);
}

// show big map
void	update_M(t_Minimap *minimap, short val)
{
	minimap->map_draw_flag = val;
}

// rotating left
void	update_Arrow_Left(t_Player *player)
{
	player->angle -= PLAYER_ROTATE_SPEED;
	if (player->angle < 0) // full rotation is done, must bring it back to start (which is already at the start kinda)
		player->angle += 2 * M_PI; // I can't just set angle to '0', it won't be smooth and maybe not correct
	// Here we calculate the direction, so we know on which direction player is watching.
	// Delta X and Y will be updated and will added to initial position of player.
	// It will change depending on what angle we are currently in.
	player->delta.x = cos(player->angle) * PLAYER_MOVE_SPEED;
	player->delta.y = sin(player->angle) * PLAYER_MOVE_SPEED;
}

// rotating right
void	update_Arrow_Right(t_Player *player)
{
	// same logic here but we add angle here
	player->angle += PLAYER_ROTATE_SPEED;
	if (player->angle > 2 * M_PI)
		player->angle -= 2 * M_PI;
	player->delta.x = cos(player->angle) * PLAYER_MOVE_SPEED;
	player->delta.y = sin(player->angle) * PLAYER_MOVE_SPEED;
}

void	update_check(t_GameData *data)
{
	if (!data)
		return ;
	if (data->input.w == B_TRUE)
		update_W(&data->player);
	if (data->input.s == B_TRUE)
		update_S(&data->player);
	if (data->input.a == B_TRUE)
		update_A(&data->player);
	if (data->input.d == B_TRUE)
		update_D(&data->player);
	if (data->input.m == B_TRUE)
		update_M(&data->minimap, B_TRUE);
	else
		update_M(&data->minimap, B_FALSE);
	if (data->input.arrow_left == B_TRUE)
		update_Arrow_Left(&data->player);
	if (data->input.arrow_right == B_TRUE)
		update_Arrow_Right(&data->player);
}