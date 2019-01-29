#ifndef __COMPILER_BACKEND_PARSER_H__
#define __COMPILER_BACKEND_PARSER_H__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../../parser.h"
#include "../../string.h"

#define	BE_NODE_INVALID				0x0000

#define	BE_NODE_MODULE				0x0100

#define	BE_NODE_IDENTIFIER			0x0200

#define	BE_NODE_LITERAL_UINT		0x0300
#define	BE_NODE_LITERAL_REAL		0x0301
#define	BE_NODE_LITERAL_CHAR		0x0302
#define	BE_NODE_LITERAL_STRING		0x0303

#define	BE_NODE_TYPE				0x0400

#define	BE_NODE_TYPE_CHAR			0x0400
#define	BE_NODE_TYPE_INT8			0x0401
#define	BE_NODE_TYPE_INT16			0x0402
#define	BE_NODE_TYPE_INT32			0x0403
#define	BE_NODE_TYPE_INT64			0x0404
#define	BE_NODE_TYPE_UINT8			0x0405
#define	BE_NODE_TYPE_UINT16			0x0406
#define	BE_NODE_TYPE_UINT32			0x0407
#define	BE_NODE_TYPE_UINT64			0x0408
#define	BE_NODE_TYPE_FLOAT			0x0409
#define	BE_NODE_TYPE_DOUBLE			0x040a

#define	BE_NODE_TYPE_STRUCT			0x0410

#define	BE_NODE_TYPE_ARRAY			0x0420
#define	BE_NODE_TYPE_ARRAY_DIMS		0x0421

#define	BE_NODE_TYPE_FUNC			0x0430

#define	BE_NODE_TYPE_POINTER		0x0440

#define	BE_NODE_TYPE_VOID			0x04ff


#define	BE_NODE_DUMMY				0x0900

#define	BE_NODE_FUNC						0x0a00
#define	BE_NODE_FUNC_PARAMS					0x0a01
#define	BE_NODE_FUNC_PARAMS_ITEM			0x0a02
#define	BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM	0x0a03




#define	BE_NODE_STATS_BLOCK					0x0b00

#define	BE_NODE_STATS_BR					0x0b10
#define	BE_NODE_STATS_BR_COND				0x0b11

#define	BE_NODE_STAT_RETURN					0x0b20

#define	BE_NODE_STAT_LABEL					0x0b30

#define	BE_NODE_STAT_ASM					0x0b40
#define	BE_NODE_STAT_ASM_OPTION				0x0b41

#define	BE_NODE_STAT_EXPR					0x0b50

#define	BE_NODE_STAT_DUMMY					0x0b60




#define	BE_NODE_VAR							0x0c00
#define	BE_NODE_VAR_ITEM					0x0c01




#define	BE_NODE_STRUCT						0x0d00
#define	BE_NODE_STRUCT_BODY					0x0d01




#define	FE_NODE_EXPR						0x0e00







#endif
