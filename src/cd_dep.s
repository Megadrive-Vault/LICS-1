		.data
		.align  2
| short set_sr(short new_sr);
| set SR, return previous SR
| entry: arg = SR value
| exit:  d0 = previous SR value
		.global set_sr
set_sr:
		moveq   #0,d0
		move.w  sr,d0
		move.l  4(sp),d1
		move.w  d1,sr
		rts
	