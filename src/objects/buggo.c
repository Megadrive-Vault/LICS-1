#include "buggo.h"
#include "vramslots.h"
#include "player.h"
#include "map.h"
#include "projectiles.h"
#include "system.h"
#include "save.h"

static void en_anim_buggo(void *v);
static void en_proc_buggo(void *v);
static void en_cube_buggo(void *v, cube *c);

void en_init_buggo(en_buggo *e, u16 type)
{
	e->head.width = BUGGO_WIDTH;
	e->head.height = BUGGO_HEIGHT;
	e->head.direction = ENEMY_LEFT;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.attr[1] = 0;
	e->head.x += 8;
	e->head.y += 8;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->head.anim_func = &en_anim_buggo;
	e->head.proc_func = &en_proc_buggo;
	if (type == BUGGO_T2)
	{
		// Custom "kicked cubes only" response function
		e->head.cube_func = &en_cube_buggo;
		e->head.yoff[0] = -4;
	}
	else
	{
		e->head.cube_func = 0;
		e->head.yoff[0] = -8;
	}

	e->h_cnt = 0;
	e->h_rev_cnt = BUGGO_DISTANCE;
	e->shot_clock = 0;
	e->spin_cnt = 0;
	e->anim_cnt = 0;
	e->type = type;

	e->head.hp = (type == BUGGO_T1) ? BUGGO_HP_1 : BUGGO_HP_2;
}

static void en_cube_buggo(void *v, cube *c)
{
	en_buggo *e = (en_buggo *)v;
	if (c->state == CUBE_STATE_KICKED)
	{
		if (c->type == CUBE_PHANTOM && sram.have_double_phantom)
		{
			// Let the enemy get hurt extra
			if (e->head.hp > 1)
			{
				e->head.hp--;
			}
		}
		if (c->type == CUBE_RED)
		{	
			if (e->head.hp > 1)
			{
				e->head.hp--;
			}
			// Let the enemy get hurt extra
			if (e->head.hp > 1)
			{
				e->head.hp--;
			}
		}
	}
	if (e->head.hurt_cnt != 0)
	{
		return;	
	}
	if (c->state == CUBE_STATE_KICKED)
	{
		enemy_get_hurt((en_generic *)e);
	}
	if (c->type == CUBE_GREEN)
	{
		c->state = CUBE_STATE_AIR;
		if (c->y < e->head.y)
		{
			c->dy = -1 * (c->dy >> 1);
		}
		else
		{
			c->dy = -CUBE_ON_CUBE_DY;
		}
		if (c->dx == FZERO)
		{
			c->dx = (GET_HVCOUNTER % 2) ? 1 : -1;
		}
		else
		{
			c->dx = c->dx * -1;
			cube_clamp_dx(c);
		}
	}
	else if (c->state != CUBE_STATE_EXPLODE && c->state != CUBE_STATE_FIZZLE)
	{
		cube_destroy(c);	
	}
}

static u16 walking_frame(u16 anim_cnt)
{
	if (anim_cnt < (BUGGO_ANIM_T / 4))
	{
		return 0;
	}
	else if (anim_cnt < (2 * BUGGO_ANIM_T / 4))
	{
		return 4;
	}
	else if (anim_cnt < (3 * BUGGO_ANIM_T / 4))
	{
		return 8;
	}
	else
	{
		return 12;
	}
}

static void en_anim_buggo(void *v)
{
	en_buggo *e = (en_buggo *)v;

	if (e->anim_cnt >= BUGGO_ANIM_T)
	{
		e->anim_cnt = 0;
	}
	else
	{
		e->anim_cnt++;
	}

	e->head.xoff[0] = -8;
	if (e->type == BUGGO_T1)
	{
		// When shot_clock <= test, walking animation
		if (e->shot_clock <= BUGGO_SHOT_TEST)
		{
			u16 frame = walking_frame(e->anim_cnt);
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, BUGGO_VRAM_SLOT + frame);
		}
		// When shot_clock > test, vibrate back and forth on frame 2
		else
		{
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, BUGGO_VRAM_SLOT + 8);
			e->head.xoff[0] += ((system_osc >> 1) % 2);
		}
	}
	else
	{
		// Normal walking animation when spin_cnt == 0
		if (e->spin_cnt == 0)
		{
			u16 frame = walking_frame(e->anim_cnt);
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, BUGGO_VRAM_SLOT + 16 + frame);
		}
		// Spinning animation
		else
		{
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, BUGGO_VRAM_SLOT + 32 + (((system_osc >> 2) % 4) << 2));
		}
	}
}

static inline void buggo_shot_proc(en_buggo *e)
{
	e->shot_clock++;
	if (e->type == BUGGO_T1)
	{
		// Shooting amount reached. Reset counter, fire projectile
		if (e->shot_clock == BUGGO_SHOT_TEST)
		{
			// Player nearby; allow a shot instead of resetting
			if (e->head.x < pl.px + BUGGO_PLAYER_SENSE &&
			    e->head.x > pl.px - BUGGO_PLAYER_SENSE &&
			    e->head.y < pl.py)
			{
				e->shot_clock++;
			}
			// No player. Reset the counter.
			else
			{
				e->shot_clock = 0;
			}
		}
		else if (e->shot_clock >= BUGGO_SHOT_FIRE)
		{
			e->shot_clock = 0;
			projectile_shoot(e->head.x, e->head.y - 4, FIX16(0.0), BUGGO_SPIKE_SPEED, PROJECTILE_SPIKE);
		}
	}
	else
	{
		if (e->spin_cnt > 0)
		{
			e->spin_cnt--;
		}
		if (e->shot_clock == BUGGO_SPARK_T)
		{
			e->shot_clock = 0;
			e->spin_cnt = BUGGO_SPIN_T;
			projectile_shoot(e->head.x, e->head.y + 7, BUGGO_SPARK_SPEED, FIX16(0.0), PROJECTILE_SPARK);
			projectile_shoot(e->head.x, e->head.y + 7, -BUGGO_SPARK_SPEED, FIX16(0.0), PROJECTILE_SPARK);
		}
	}
}

static inline void bg_collision(en_buggo *e)
{
	if (e->head.direction == ENEMY_RIGHT && 
	    map_collision(e->head.x + BUGGO_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_LEFT;	
		e->head.x -= 4;
		e->h_rev_cnt = 100;
	}
	else if (e->head.direction == ENEMY_LEFT &&
	    map_collision(e->head.x - BUGGO_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_RIGHT;
		e->head.x += 4;
		e->h_rev_cnt = 0;
	}
}

static inline int buggo_can_move(en_buggo *e)
{
	if (e->type == BUGGO_T1)
	{
		if (e->shot_clock > BUGGO_SHOT_TEST)
		{
			return 0;
		}
	}
	else
	{
		if (e->spin_cnt > 0)
		{
			return 0;
		}
	}
	return 1;
}

static inline void h_movement(en_buggo *e)
{
	if (!buggo_can_move(e))
	{
		return;
	}
	if (e->h_cnt == BUGGO_H_CNT_MAX)
	{
		if (e->head.direction == ENEMY_RIGHT)
		{
			e->head.x++;
		}
		else
		{
			e->head.x--;
		}
		e->h_cnt = 0;

		if (e->h_rev_cnt == BUGGO_DISTANCE * 2)
		{
			e->h_rev_cnt = 0;
			e->head.direction = ENEMY_RIGHT;
		}
		else if (e->h_rev_cnt == BUGGO_DISTANCE)
		{
			e->head.direction = ENEMY_LEFT;
		}

		e->h_rev_cnt++;
	}
	else
	{
		e->h_cnt++;
	}
	bg_collision(e);
}

static void en_proc_buggo(void *v)
{
	en_buggo *e = (en_buggo *)v;
	h_movement(e);
	buggo_shot_proc(e);
}
