/*BIT16;
HINT(opcode_o1=0x91, opcode_len=1) XCHG AX, CX;
HINT(opcode_o1=0x91, opcode_len=1) XCHG CX, AX;
XCHG EAX, EAX;

XCHG AX, CX;
XCHG EAX, ECX;

BIT32;
XCHG AX, AX;
XCHG EAX, EAX;

XCHG AX, CX;
XCHG EAX, ECX;*/

/*BIT64;

XCHG AX, AX;
XCHG EAX, EAX;
XCHG RAX, RAX;

XCHG AX, R8W;
XCHG EAX, R8D;
XCHG RAX, R8;


XCHG AX, CX;
XCHG EAX, ECX;
XCHG RAX, RCX;

XCHG AX, R9W;
XCHG EAX, R9D;
XCHG RAX, R9;*/

/*
BIT32;

MOV	AL, BYTE ADDR32 [0x1234];
MOV	AX, WORD ADDR32 [0x1234];
MOV	EAX, DWORD ADDR32 [0x1234];
*/

BIT64;
MOV AL, 0x12;
MOV R8B, 0x34;
