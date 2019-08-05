#pragma once

// prefix explaination:
// P: points to a pointer
// F: points to a function
// A: points to an address

// get info
#define USERNAME_ADDR_OFFSET		0x126d91c
#define WECHATID_ADDR_OFFSET		0x126da80
#define P_AVARTARURL_ADDR_OFFSET	0x126dbe4

// send message
#define F_SENDMSG_ADDR_OFFSET		0x2eb4e0

// receive message
#define A_RETCALL_ADDR_OFFSET		0x126D7F8
#define A_RETCALL2_ADDR_OFFSET		0x2599D0
#define A_RET_ADDR_OFFSET			0x315E9B
#define A_WXID_OFFSET_TO_ESI_NEG	0x1A0	
#define A_MESSAGE_OFFSET_TO_ESI_NEG	0x178
#define A_WXID2_OFFSET_TO_ESI_NEG	0xCC
#define A_UNKNOWN_OFFSET_TO_ESI_NEG	0xB8
#define A_HOOK_ADDR_OFFSET			0x315E93
