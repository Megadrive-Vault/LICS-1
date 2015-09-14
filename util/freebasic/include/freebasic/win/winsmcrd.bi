'' FreeBASIC binding for mingw-w64-v4.0.1
''
'' based on the C header files:
''   DISCLAIMER
''   This file has no copyright assigned and is placed in the Public Domain.
''   This file is part of the mingw-w64 runtime package.
''
''   The mingw-w64 runtime package and its code is distributed in the hope that it 
''   will be useful but WITHOUT ANY WARRANTY.  ALL WARRANTIES, EXPRESSED OR 
''   IMPLIED ARE HEREBY DISCLAIMED.  This includes but is not limited to 
''   warranties of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
''
'' translated to FreeBASIC by:
''   Copyright © 2015 FreeBASIC development team

#pragma once

#include once "_mingw_unicode.bi"

#define _NTDDSCRD_H2_
type UWORD as WORD
const FILE_DEVICE_SMARTCARD = &h00000031
const SCARD_ATR_LENGTH = 33
const SCARD_PROTOCOL_UNDEFINED = &h00000000
const SCARD_PROTOCOL_T0 = &h00000001
const SCARD_PROTOCOL_T1 = &h00000002
const SCARD_PROTOCOL_RAW = &h00010000
#define SCARD_PROTOCOL_Tx (SCARD_PROTOCOL_T0 or SCARD_PROTOCOL_T1)
const SCARD_PROTOCOL_DEFAULT = &h80000000
const SCARD_PROTOCOL_OPTIMAL = &h00000000
const SCARD_POWER_DOWN = 0
const SCARD_COLD_RESET = 1
const SCARD_WARM_RESET = 2
#define SCARD_CTL_CODE(code) CTL_CODE(FILE_DEVICE_SMARTCARD, (code), METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SMARTCARD_POWER SCARD_CTL_CODE(1)
#define IOCTL_SMARTCARD_GET_ATTRIBUTE SCARD_CTL_CODE(2)
#define IOCTL_SMARTCARD_SET_ATTRIBUTE SCARD_CTL_CODE(3)
#define IOCTL_SMARTCARD_CONFISCATE SCARD_CTL_CODE(4)
#define IOCTL_SMARTCARD_TRANSMIT SCARD_CTL_CODE(5)
#define IOCTL_SMARTCARD_EJECT SCARD_CTL_CODE(6)
#define IOCTL_SMARTCARD_SWALLOW SCARD_CTL_CODE(7)
#define IOCTL_SMARTCARD_IS_PRESENT SCARD_CTL_CODE(10)
#define IOCTL_SMARTCARD_IS_ABSENT SCARD_CTL_CODE(11)
#define IOCTL_SMARTCARD_SET_PROTOCOL SCARD_CTL_CODE(12)
#define IOCTL_SMARTCARD_GET_STATE SCARD_CTL_CODE(14)
#define IOCTL_SMARTCARD_GET_LAST_ERROR SCARD_CTL_CODE(15)
#define IOCTL_SMARTCARD_GET_PERF_CNTR SCARD_CTL_CODE(16)
const MAXIMUM_ATTR_STRING_LENGTH = 32
const MAXIMUM_SMARTCARD_READERS = 10
#define SCARD_ATTR_VALUE(Class, Tag) ((cast(ULONG, (Class)) shl 16) or cast(ULONG, (Tag)))
const SCARD_CLASS_VENDOR_INFO = 1
const SCARD_CLASS_COMMUNICATIONS = 2
const SCARD_CLASS_PROTOCOL = 3
const SCARD_CLASS_POWER_MGMT = 4
const SCARD_CLASS_SECURITY = 5
const SCARD_CLASS_MECHANICAL = 6
const SCARD_CLASS_VENDOR_DEFINED = 7
const SCARD_CLASS_IFD_PROTOCOL = 8
const SCARD_CLASS_ICC_STATE = 9
const SCARD_CLASS_PERF = &h7ffe
const SCARD_CLASS_SYSTEM = &h7fff
#define SCARD_ATTR_VENDOR_NAME SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_INFO, &h0100)
#define SCARD_ATTR_VENDOR_IFD_TYPE SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_INFO, &h0101)
#define SCARD_ATTR_VENDOR_IFD_VERSION SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_INFO, &h0102)
#define SCARD_ATTR_VENDOR_IFD_SERIAL_NO SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_INFO, &h0103)
#define SCARD_ATTR_CHANNEL_ID SCARD_ATTR_VALUE(SCARD_CLASS_COMMUNICATIONS, &h0110)
#define SCARD_ATTR_PROTOCOL_TYPES SCARD_ATTR_VALUE(SCARD_CLASS_PROTOCOL, &h0120)
#define SCARD_ATTR_DEFAULT_CLK SCARD_ATTR_VALUE(SCARD_CLASS_PROTOCOL, &h0121)
#define SCARD_ATTR_MAX_CLK SCARD_ATTR_VALUE(SCARD_CLASS_PROTOCOL, &h0122)
#define SCARD_ATTR_DEFAULT_DATA_RATE SCARD_ATTR_VALUE(SCARD_CLASS_PROTOCOL, &h0123)
#define SCARD_ATTR_MAX_DATA_RATE SCARD_ATTR_VALUE(SCARD_CLASS_PROTOCOL, &h0124)
#define SCARD_ATTR_MAX_IFSD SCARD_ATTR_VALUE(SCARD_CLASS_PROTOCOL, &h0125)
#define SCARD_ATTR_POWER_MGMT_SUPPORT SCARD_ATTR_VALUE(SCARD_CLASS_POWER_MGMT, &h0131)
#define SCARD_ATTR_USER_TO_CARD_AUTH_DEVICE SCARD_ATTR_VALUE(SCARD_CLASS_SECURITY, &h0140)
#define SCARD_ATTR_USER_AUTH_INPUT_DEVICE SCARD_ATTR_VALUE(SCARD_CLASS_SECURITY, &h0142)
#define SCARD_ATTR_CHARACTERISTICS SCARD_ATTR_VALUE(SCARD_CLASS_MECHANICAL, &h0150)
#define SCARD_ATTR_CURRENT_PROTOCOL_TYPE SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0201)
#define SCARD_ATTR_CURRENT_CLK SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0202)
#define SCARD_ATTR_CURRENT_F SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0203)
#define SCARD_ATTR_CURRENT_D SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0204)
#define SCARD_ATTR_CURRENT_N SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0205)
#define SCARD_ATTR_CURRENT_W SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0206)
#define SCARD_ATTR_CURRENT_IFSC SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0207)
#define SCARD_ATTR_CURRENT_IFSD SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0208)
#define SCARD_ATTR_CURRENT_BWT SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h0209)
#define SCARD_ATTR_CURRENT_CWT SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h020a)
#define SCARD_ATTR_CURRENT_EBC_ENCODING SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h020b)
#define SCARD_ATTR_EXTENDED_BWT SCARD_ATTR_VALUE(SCARD_CLASS_IFD_PROTOCOL, &h020c)
#define SCARD_ATTR_ICC_PRESENCE SCARD_ATTR_VALUE(SCARD_CLASS_ICC_STATE, &h0300)
#define SCARD_ATTR_ICC_INTERFACE_STATUS SCARD_ATTR_VALUE(SCARD_CLASS_ICC_STATE, &h0301)
#define SCARD_ATTR_CURRENT_IO_STATE SCARD_ATTR_VALUE(SCARD_CLASS_ICC_STATE, &h0302)
#define SCARD_ATTR_ATR_STRING SCARD_ATTR_VALUE(SCARD_CLASS_ICC_STATE, &h0303)
#define SCARD_ATTR_ICC_TYPE_PER_ATR SCARD_ATTR_VALUE(SCARD_CLASS_ICC_STATE, &h0304)
#define SCARD_ATTR_ESC_RESET SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_DEFINED, &hA000)
#define SCARD_ATTR_ESC_CANCEL SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_DEFINED, &hA003)
#define SCARD_ATTR_ESC_AUTHREQUEST SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_DEFINED, &hA005)
#define SCARD_ATTR_MAXINPUT SCARD_ATTR_VALUE(SCARD_CLASS_VENDOR_DEFINED, &hA007)
#define SCARD_ATTR_DEVICE_UNIT SCARD_ATTR_VALUE(SCARD_CLASS_SYSTEM, &h0001)
#define SCARD_ATTR_DEVICE_IN_USE SCARD_ATTR_VALUE(SCARD_CLASS_SYSTEM, &h0002)
#define SCARD_ATTR_DEVICE_FRIENDLY_NAME_A SCARD_ATTR_VALUE(SCARD_CLASS_SYSTEM, &h0003)
#define SCARD_ATTR_DEVICE_SYSTEM_NAME_A SCARD_ATTR_VALUE(SCARD_CLASS_SYSTEM, &h0004)
#define SCARD_ATTR_DEVICE_FRIENDLY_NAME_W SCARD_ATTR_VALUE(SCARD_CLASS_SYSTEM, &h0005)
#define SCARD_ATTR_DEVICE_SYSTEM_NAME_W SCARD_ATTR_VALUE(SCARD_CLASS_SYSTEM, &h0006)
#define SCARD_ATTR_SUPRESS_T1_IFS_REQUEST SCARD_ATTR_VALUE(SCARD_CLASS_SYSTEM, &h0007)
#define SCARD_PERF_NUM_TRANSMISSIONS SCARD_ATTR_VALUE(SCARD_CLASS_PERF, &h0001)
#define SCARD_PERF_BYTES_TRANSMITTED SCARD_ATTR_VALUE(SCARD_CLASS_PERF, &h0002)
#define SCARD_PERF_TRANSMISSION_TIME SCARD_ATTR_VALUE(SCARD_CLASS_PERF, &h0003)

#ifdef UNICODE
	#define SCARD_ATTR_DEVICE_FRIENDLY_NAME SCARD_ATTR_DEVICE_FRIENDLY_NAME_W
	#define SCARD_ATTR_DEVICE_SYSTEM_NAME SCARD_ATTR_DEVICE_SYSTEM_NAME_W
#else
	#define SCARD_ATTR_DEVICE_FRIENDLY_NAME SCARD_ATTR_DEVICE_FRIENDLY_NAME_A
	#define SCARD_ATTR_DEVICE_SYSTEM_NAME SCARD_ATTR_DEVICE_SYSTEM_NAME_A
#endif

const SCARD_T0_HEADER_LENGTH = 7
const SCARD_T0_CMD_LENGTH = 5
const SCARD_T1_PROLOGUE_LENGTH = 3
const SCARD_T1_EPILOGUE_LENGTH = 2
const SCARD_T1_MAX_IFS = 254
const SCARD_UNKNOWN = 0
const SCARD_ABSENT = 1
const SCARD_PRESENT = 2
const SCARD_SWALLOWED = 3
const SCARD_POWERED = 4
const SCARD_NEGOTIABLE = 5
const SCARD_SPECIFIC = 6

type _SCARD_IO_REQUEST
	dwProtocol as DWORD
	cbPciLength as DWORD
end type

type SCARD_IO_REQUEST as _SCARD_IO_REQUEST
type PSCARD_IO_REQUEST as _SCARD_IO_REQUEST ptr
type LPSCARD_IO_REQUEST as _SCARD_IO_REQUEST ptr
type LPCSCARD_IO_REQUEST as const SCARD_IO_REQUEST ptr

type SCARD_T0_COMMAND
	bCla as UBYTE
	bIns as UBYTE
	bP1 as UBYTE
	bP2 as UBYTE
	bP3 as UBYTE
end type

type LPSCARD_T0_COMMAND as SCARD_T0_COMMAND ptr

type SCARD_T0_REQUEST
	ioRequest as SCARD_IO_REQUEST
	bSw1 as UBYTE
	bSw2 as UBYTE

	union
		CmdBytes as SCARD_T0_COMMAND
		rgbHeader(0 to 4) as UBYTE
	end union
end type

type PSCARD_T0_REQUEST as SCARD_T0_REQUEST ptr
type LPSCARD_T0_REQUEST as SCARD_T0_REQUEST ptr

type SCARD_T1_REQUEST
	ioRequest as SCARD_IO_REQUEST
end type

type PSCARD_T1_REQUEST as SCARD_T1_REQUEST ptr
type LPSCARD_T1_REQUEST as SCARD_T1_REQUEST ptr
const SCARD_READER_SWALLOWS = &h00000001
const SCARD_READER_EJECTS = &h00000002
const SCARD_READER_CONFISCATES = &h00000004
const SCARD_READER_TYPE_SERIAL = &h01
const SCARD_READER_TYPE_PARALELL = &h02
const SCARD_READER_TYPE_KEYBOARD = &h04
const SCARD_READER_TYPE_SCSI = &h08
const SCARD_READER_TYPE_IDE = &h10
const SCARD_READER_TYPE_USB = &h20
const SCARD_READER_TYPE_PCMCIA = &h40
const SCARD_READER_TYPE_VENDOR = &hF0
