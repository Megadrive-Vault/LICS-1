#include "system.h"
// ----------------
#include "sprites.h"
#include "mpad.h"
#include "save.h"

static vu16 vbl_active;
u16 system_osc;

static _voidCallback *v_int(void)
{
	vbl_active = 1;
	return NULL;
}

static _voidCallback *h_int(void)
{
	return NULL;
}

void system_init(void)
{
	VDP_init();
	// Configure interrupts
	SYS_disableInts();
	VDP_setHInterrupt(1);
	VDP_setHIntCounter(223 - 6);
	SYS_setVIntCallback((_voidCallback *)v_int);
	SYS_setHIntCallback((_voidCallback *)h_int);
	SYS_enableInts();

	// Set up basic VDP settings
	VDP_setPlanSize(SYSTEM_PLANE_W,SYSTEM_PLANE_H);
	VDP_setScreenWidth320();
	VDP_setScreenHeight240();
	VDP_setHilightShadow(0);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

	sprites_init();

	save_load();
	// Allow holding A + Start to change scan modes
	if ((pad_read(0) & KEY_A) || (pad_read(0) & KEY_START))
	{
		sram.opt_interlace = (sram.opt_interlace == SAVE_OPT_INTERLACE_ENABLED) ? (SAVE_OPT_INTERLACE_NORMAL) : (SAVE_OPT_INTERLACE_ENABLED);
		save_write();
	}
	if (sram.opt_interlace == SAVE_OPT_INTERLACE_ENABLED)
	{
		VDP_setScanMode(INTERLACED_MODE1);
	}
	else
	{
		VDP_setScanMode(INTERLACED_NONE);
	}

}

void system_wait_v(void)
{
	system_osc++;
	while (!vbl_active)
	{
		// Twiddle thumbs
	}
	vbl_active = 0;
}

