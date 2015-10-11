#include "cubes.h"
#include "vramslots.h"
#include "gfx.h"
#include "sprites.h"
#include "state.h"
#include "system.h"
#include "player.h"
#include "map.h"
#include "particles.h"
#include "music.h"
#include "enemy.h"
#include "powerups.h"
#include "save.h"

cube cubes[CUBES_NUM];

static void cube_move(cube *c);
static void cube_clamp_dx(cube *c);
static void cube_degrade_dx(cube *c);
static void cube_on_cube_collisions(cube *c);
static void cube_eval_stopmoving(cube *c);
static void cube_do_ground_recoil(cube *c);
static void cube_bg_bounce_sides(cube *c);
static void cube_bg_bounce_top(cube *c);
static void cube_bg_bounce_ground(cube *c);
static void cube_bg_collision(cube *c);

void cube_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_cubes, CUBE_VRAM_SLOT * 32,CUBE_VRAM_LEN * 16);
	// Cubes use Lyle's palette. No palette transfer here.
}

void cubes_init(void)
{
	int i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		c->state = CUBE_STATE_INACTIVE;
	}
}

static void cube_destroy(cube *c)
{
	c->dx = 0;
	c->dy = 7;
	if (c->type == CUBE_RED)
	{
		c->state = CUBE_STATE_EXPLODE;
		playsound(SFX_CUBE_EXPLODE);
	}
	else
	{
		c->state = CUBE_STATE_FIZZLE;
		playsound(SFX_FIZZLE);
	}

	if ((c->type & 0xFF00) == CUBE_YELLOW)
	{
		// Spawn a powerup of the correct type, as needed
		switch (c->type & 0x00F0)
		{
			default:
			case CUBE_YELLOW_HP:
				powerup_spawn(c->x, c->y, POWERUP_HP, 0);
				break;
			case CUBE_YELLOW_HP_2X:
				powerup_spawn(c->x, c->y, POWERUP_HP_2X, 0);
				break;
			case CUBE_YELLOW_CP:
				powerup_spawn(c->x, c->y, POWERUP_CP, 0);
				break;
			case CUBE_YELLOW_CP_2X:
				powerup_spawn(c->x, c->y, POWERUP_CP_2X, 0);
				break;
			case CUBE_YELLOW_CPORB:
				// Did we collect this # orb already?
				if (!sram.cp_orbs_taken[c->type & 0x000F])
				{
					// If not, spawn it, assign it the tag
					powerup_spawn(c->x, c->y, POWERUP_CPORB, c->type & 0x000F);	
				}
				else
				{
					// Otherwise, spawn a CP 2X powerup
					powerup_spawn(c->x, c->y, POWERUP_CP_2X, 0);
				}
				break;
			case CUBE_YELLOW_HPORB:
				// Did we collect this # orb already?
				if (!sram.hp_orbs_taken[c->type & 0x000F])
				{
					// Spawn it with HP orb tag
					powerup_spawn(c->x, c->y, POWERUP_HPORB, c->type & 0x000F);	
				}
				else
				{
					// Give an HP powerup instead
					powerup_spawn(c->x, c->y, POWERUP_HP_2X, 0);
				}
				break;
		}
	}
}

void cube_delete(cube *c)
{
	c->type = NULL;
	c->state = CUBE_STATE_INACTIVE;
	c->x = -32;
	c->y = -32;
}

static void cube_move(cube *c)
{
	// Y movement only applies to an airborn cube
	if (c->state == CUBE_STATE_AIR)
	{	
		c->y += fix16ToInt(c->dy);
		c->dy = fix16Add(c->dy, CUBE_GRAVITY);
	}
	c->x += c->dx;
	if (c->state == CUBE_STATE_KICKED)
	{
		// in free air, change to an air cube and fall straight down
		if (!map_collision(c->x + CUBE_LEFT, c->y + CUBE_BOTTOM + 1) &&
			!map_collision(c->x + CUBE_RIGHT, c->y + CUBE_BOTTOM + 1))
		{
			c->dx = 0;
			c->state = CUBE_STATE_AIR;
			c->dy = CUBE_GRAVITY;
			// lock to grid
			c->x = ((c->x + 4) / 8) * 8;
		}
	}
	// Check for cube out of bounds
	if (c->x + CUBE_RIGHT > state.current_room->w * STATE_SC_W || 
		c->x + CUBE_LEFT < 0 ||
		c->y + CUBE_BOTTOM > state.current_room->h * STATE_SC_H || 
		c->y + CUBE_TOP < 0)
	{
		c->state = CUBE_STATE_INACTIVE;
		c->x = -32;
		c->y = -32;
	}
}

static void cube_degrade_dx(cube *c)
{
	if (c->dx > 0)
	{
		c->dx--;
	}
	else if (c->dx < 0)
	{
		c->dx++;
	}
}

static void cube_clamp_dx(cube *c)
{
	if (c->dx > 0)
	{
		c->dx = CUBE_ON_CUBE_DX;
	}
	else if (c->dx < 0)
	{
		c->dx = CUBE_ON_CUBE_DX * -1;
	}
}

static void cube_on_cube_collisions(cube *c)
{
	if (c->cube_col_timeout)
	{
		c->cube_col_timeout--;
		return;
	}
	int i = CUBES_NUM;
	while (i--)
	{
		cube *d = &cubes[i];
		if (d->state == CUBE_STATE_INACTIVE || d->state == CUBE_STATE_FIZZLE || d == c)
		{
			continue;
		}
		if (c->type != CUBE_GREEN)
		{
			if (c->x + CUBE_LEFT <= d->x + CUBE_RIGHT && 
				c->x + CUBE_RIGHT >= d->x + CUBE_LEFT && 
				c->y + CUBE_TOP <= d->y + CUBE_BOTTOM &&
				c->y + CUBE_BOTTOM >= d->y + CUBE_TOP)
			{
				if (c->state != CUBE_STATE_IDLE)
				{
					cube_destroy(c);
				}
				else if (c->dy != FZERO)
				{
					c->dy = FZERO;
				}
				if (d->type != CUBE_GREEN && d->state != CUBE_STATE_IDLE)
				{
					cube_destroy(d);
				}
				else if (d->dy != FZERO)
				{
					d->dy = FZERO;
				}
			}
		}
		else
		{
			if (c->x + CUBE_LEFT <= d->x + CUBE_RIGHT && 
				c->x + CUBE_RIGHT >= d->x + CUBE_LEFT && 
				c->y + CUBE_TOP <= d->y + CUBE_BOTTOM &&
				c->y + CUBE_BOTTOM >= d->y + CUBE_TOP)
			{
				if (c->state != CUBE_STATE_IDLE)
				{
					if (c->state == CUBE_STATE_KICKED)
					{
						c->dx = c->dx * -1;
						c->state = CUBE_STATE_AIR;
						c->bounce_count = CUBE_BOUNCE_COUNT_INIT;
					}
					if (c->dx)
					{
						cube_clamp_dx(c);
					}
					else
					{
						c->dx = GET_HVCOUNTER % 2 ? 1 : -1;
					}
					c->dy = CUBE_ON_CUBE_DY;
					playsound(SFX_CUBEBOUNCE);
					c->cube_col_timeout = CUBE_COL_T;
				}
				else if (c->dy != FZERO)
				{
					c->dy = FZERO;
				}
				if (d->type != CUBE_GREEN && d->state != CUBE_STATE_IDLE)
				{
					cube_destroy(d);
				}
				else if (d->dy != FZERO)
				{
					d->dy = FZERO;
				}
			}
		}
	}
}

static void cube_eval_stopmoving(cube *c)
{
	if (c->bounce_count <= 0 && c->dx == 0)
	{
		c->dy = FZERO;
		c->y = ((c->y / 8) * 8) - 1;
		c->state = CUBE_STATE_IDLE;
	}
	else if (c->bounce_count == 0)
	{
		c->bounce_count = 1;
	}

	if (c->dx == 0 && c->dy > CUBE_BOUNCE_CUTOFF)
	{
		c->bounce_count--;
	}
	else
	{
		c->bounce_count = CUBE_BOUNCE_COUNT_INIT;
	}
}

static void cube_do_ground_recoil(cube *c)
{
	// First push the cube out of the ground if it's stuck
	c->y = (c->y / 8) * 8;
	c->dy = fix16Mul(c->dy, CUBE_BOUNCE_COEF);
	c->dy = c->dy - (c->dy + c->dy);
	cube_degrade_dx(c);
	cube_eval_stopmoving(c);
	playsound(SFX_CUBEBOUNCE);
}

static void cube_bg_bounce_ground(cube *c)
{	
	// Check the left and right bottom corners of the cube respectively
	u16 gnd_chk[2];
	gnd_chk[0] = map_collision(c->x + CUBE_LEFT, c->y + CUBE_BOTTOM);
	gnd_chk[1] = map_collision(c->x + CUBE_RIGHT, c->y + CUBE_BOTTOM);
	// Both left and right test passes for being on the ground
	if (gnd_chk[0] && gnd_chk[1])
	{
		cube_do_ground_recoil(c);
	}
	else if (gnd_chk[0] || gnd_chk[1])
	{
		// One edge is missing. Check the center.
		u16 cnt_chk = map_collision(c->x, c->y + CUBE_BOTTOM);
		// Center checks out, do a normal bounce
		if (cnt_chk)
		{
			cube_do_ground_recoil(c);
		}
		// Center didn't check out. Align it to the wall it's partially on.
		else
		{
			if (gnd_chk[0])
			{
				c->x = ((c->x + 8) / 8) * 8;
			}
			else
			{
				c->x = ((c->x - 2) / 8) * 8;
			}
			return;
		}
	}
}

static void cube_bg_bounce_sides(cube *c)
{
	// If we're lodged in the ground, then this isn't the time.
	if (map_collision(c->x, c->y + CUBE_BOTTOM))
	{
		return;
	}
		
	// Check walls
	u16 side_chk[2];
	if (c->dx < 0)
	{
		side_chk[0] = map_collision(c->x + CUBE_LEFT, c->y + CUBE_TOP);
		side_chk[1] = map_collision(c->x + CUBE_LEFT, c->y + CUBE_BOTTOM);
	}
	else
	{
		side_chk[0] = map_collision(c->x + CUBE_RIGHT, c->y + CUBE_TOP);
		side_chk[1] = map_collision(c->x + CUBE_RIGHT, c->y + CUBE_BOTTOM);
	}
	if (side_chk[0] || side_chk[1])
	{
		c->dx = c->dx * -1;
		cube_clamp_dx(c);
		playsound(SFX_CUBEBOUNCE);
		if (side_chk[0])
		{
			c->x = (c->x / 8) * 8;
		}
		if (c->state == CUBE_STATE_KICKED)
		{
			c->state = CUBE_STATE_AIR;
			c->dy = CUBE_ON_CUBE_DY;
			c->bounce_count = CUBE_BOUNCE_COUNT_INIT;
		}
	}
}

static void cube_bg_bounce_top(cube *c)
{
	// Check the left and right bottom corners of the cube respectively
	u16 gnd_chk[2];
	gnd_chk[0] = map_collision(c->x + CUBE_LEFT, c->y + CUBE_TOP);
	gnd_chk[1] = map_collision(c->x + CUBE_RIGHT, c->y + CUBE_TOP);
	// Both left and right test passes for being on the ground
	if (gnd_chk[0] && gnd_chk[1])
	{
		c->dy = CUBE_CEILING_DY;
		playsound(SFX_CUBEBOUNCE);
	}
	else if (gnd_chk[0] || gnd_chk[1])
	{
		// One edge is missing. Check the center.
		u16 cnt_chk = map_collision(c->x, c->y + CUBE_TOP);
		// Center checks out, do a normal bounce
		if (cnt_chk)
		{
			c->dy = CUBE_CEILING_DY;
			playsound(SFX_CUBEBOUNCE);
		}
		// Center didn't check out. Align it to the wall it's partially on.
		else
		{
			if (gnd_chk[0])
			{
				c->x = ((c->x + 8) / 8) * 8;
			}
			else
			{
				c->x = ((c->x - 2) / 8) * 8;
			}
			return;
		}
	}
}

static void cube_bg_collision(cube *c)
{
	if (map_collision(c->x + CUBE_LEFT, c->y + CUBE_BOTTOM) || 
		map_collision(c->x + CUBE_RIGHT, c->y + CUBE_BOTTOM) || 
		map_collision(c->x + CUBE_LEFT, c->y + CUBE_TOP) || 
		map_collision(c->x + CUBE_RIGHT, c->y + CUBE_TOP)) 
	{
		// Cubes that get destryed on impact
		if (c->type != CUBE_GREEN)
		{
			cube_destroy(c);
		}
		else
		{
			if (c->dx != 0)
			{
				cube_bg_bounce_sides(c);
			}
			if (c->dy > FZERO)
			{
				cube_bg_bounce_ground(c);
			}
			if (c->dy < FZERO)
			{
				cube_bg_bounce_top(c);
			}
		}
	}
}

static void cube_scan_enemies(cube *c)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *e = &enemies[i];
		if (e->head.type == ENEMY_NULL)
		{
			return;
		}
		if (e->head.active != ENEMY_ONSCREEN)
		{
			continue;
		}
		if (e->head.x - e->head.width <= c->x + CUBE_RIGHT &&
			e->head.x + e->head.width >= c->x + CUBE_LEFT &&
			e->head.y - e->head.width <= c->y + CUBE_BOTTOM &&
			e->head.y >= c->y + CUBE_TOP)
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
				}			// Let the enemy get hurt extra
				if (e->head.hp > 1)
				{
					e->head.hp--;
				}

			}
			enemy_get_hurt(e);

			if (c->type == CUBE_GREEN)
			{
				c->state = CUBE_STATE_AIR;
				if (c->y < e->head.y)
				{
					c->dy = CUBE_ON_CUBE_DY;
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
			else if (c->state != CUBE_STATE_FIZZLE)
			{
				cube_destroy(c);	
			}
		}
	}
}

// Public stuff --------------------------------------------------------------

void cubes_run(void)
{
	int i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			continue;
		}
		else if (c->state >= CUBE_STATE_FIZZLE)
		{
			if (c->dy > 0)
			{
				c->dy--;
				cube_scan_enemies(c);
			}
			else
			{
				c->state = CUBE_STATE_INACTIVE;	
				c->x = -32;
				c->y = -32;
			}
			continue;
		}
		else if (c->state != CUBE_STATE_IDLE)
		{
			// Collison is processed a frame late intentionally to mimic the
			// original game's behavior. 
			cube_scan_enemies(c);
			if (c->state != CUBE_STATE_FIZZLE)
			{
				cube_bg_collision(c);
			}
			cube_move(c);
			if (c->state == CUBE_STATE_AIR ||
				c->state == CUBE_STATE_KICKED)
			{
				cube_on_cube_collisions(c);
			}
		}
	}
}

void cubes_draw(void)
{
	int i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			continue;
		}
		else if (c->state == CUBE_STATE_FIZZLE)
		{
			particle_spawn(c->x, c->y - 8, PARTICLE_TYPE_FIZZLE);
			continue;
		}
		else if (c->state == CUBE_STATE_FIZZLERED)
		{
			particle_spawn(c->x, c->y - 8, PARTICLE_TYPE_FIZZLERED);
			continue;
		}
		else if (c->state == CUBE_STATE_EXPLODE)
		{
			particle_spawn(c->x, c->y - 8, PARTICLE_TYPE_EXPLOSION);
			continue;
		}
		s16 cx = c->x - state.cam_x + CUBE_LEFT;
		s16 cy = c->y - state.cam_y + CUBE_TOP;
		if (cx <= -16 || cx >= 320 || cy <= -16 || cy >= 240)
		{
			continue;
		}
		cube_draw_single(c->x + CUBE_LEFT, c->y + CUBE_TOP, c->type);
	}
}

void cube_draw_single(u16 x, u16 y, u16 type)
{
	u16 palnum;
	if (type == CUBE_BLUE || type == CUBE_ORANGE)
	{
		palnum = CUBE_ALT_PALNUM;
	}
	else
	{
		palnum = CUBE_PALNUM;
	}
	u16 frame = TILE_ATTR_FULL(palnum, 1, 0, 0, CUBE_VRAM_SLOT);
	switch (type & 0xFF00)
	{
		case CUBE_PHANTOM:
			frame += 16 + (4 * ((system_osc / 4) % 4));
			break;
		case CUBE_YELLOW:
			frame += 4;
			break;
		case CUBE_RED:
			frame += 8;
			break;
		case CUBE_GREEN:
			frame += 12;
			break;
	}

	sprite_put(x - state.cam_x, y - state.cam_y, SPRITE_SIZE(2,2), frame);
}

void cube_spawn(u16 x, u16 y, u16 type, u16 state, s16 dx, fix16 dy)
{
	int i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			c->state = state;
			c->x = x;
			c->y = y;
			c->dx = dx;
			c->dy = dy;
			c->type = type;
			c->cube_col_timeout = 0;
			c->bounce_count = CUBE_BOUNCE_COUNT_INIT;
			break;
		}
	}
}
