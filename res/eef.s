.section .rodata

    .align  2
    .global eef_flat
eef_flat:
    dc.w    0xFE00, 0xFF00

    .align  2
    .global eef_hat
eef_hat:
    dc.w    0x0205, 0x080A, 0x0C0D, 0xFFFF

    .align  2
    .global eef_qdecay
eef_qdecay:
    dc.w    0x0000, 0x0001, 0x0102, 0x0406, 0x0809, 0x090A, 0x0A0B, 0x0B0C
    dc.w    0x0DFE, 0x0EFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF

    .align  2
    .global eef_arp1
eef_arp1:
    dc.w    0x0000, 0x0101, 0x0202, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C
    dc.w    0x0DFE, 0x0EFF, 0xFFFF, 0xFFFF

    .align  2
    .global eef_arp2
eef_arp2:
    dc.w    0x0303, 0x0404, 0x0505, 0x0606, 0x0708, 0x090A, 0x0A0B, 0x0C0C
    dc.w    0x0DFE, 0x0EFF, 0xFFFF, 0xFFFF

