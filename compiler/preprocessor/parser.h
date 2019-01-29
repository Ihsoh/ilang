#ifndef	__COMPILER_PREPROCESSOR_PARSER_H__
#define	__COMPILER_PREPROCESSOR_PARSER_H__

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../../lexer.h"
#include "../../parser.h"

#define	PP_NODE_INVALID				0x0000

#define	PP_NODE_MODULE				0x0100

#define	PP_NODE_DIRECTIVE_INCLUDE	0x0200
#define	PP_NODE_DIRECTIVE_DEFINE	0x0201
#define	PP_NODE_DIRECTIVE_UNDEF		0x0202
#define	PP_NODE_DIRECTIVE_IF		0x0203
#define	PP_NODE_DIRECTIVE_IFDEF		0x0204
#define	PP_NODE_DIRECTIVE_IFNDEF	0x0205
#define	PP_NODE_DIRECTIVE_ELIF		0x0206
#define	PP_NODE_DIRECTIVE_ELIFDEF	0x0207
#define	PP_NODE_DIRECTIVE_ELIFNDEF	0x0208
#define	PP_NODE_DIRECTIVE_ELSE		0x0209
#define	PP_NODE_DIRECTIVE_INFO		0x020a
#define	PP_NODE_DIRECTIVE_WARN		0x020b
#define	PP_NODE_DIRECTIVE_ERROR		0x020c

#define	PP_NODE_BLOCK				0x0300
#define	PP_NODE_CMD_LINE			0x0301
#define	PP_NODE_LINE				0x0302

#define	PP_NODE_SPACE				0x0400




typedef struct {
	LexerToken		*token_endif;
} PPParserIfxxxASTNodeData;

#define	PP_IFXXX_AST_NODE_GET_TOKEN_ENDIF(node)			(((PPParserIfxxxASTNodeData *)&((node)->data[0]))->token_endif)

#define	PP_IFXXX_AST_NODE_SET_TOKEN_ENDIF(node, tk)		(((PPParserIfxxxASTNodeData *)&((node)->data[0]))->token_endif = (tk))




extern ParserContext * pp_parser_new_context(
	const char *file,
	const char *source,
	int len
);
extern void pp_parser_free_context(ParserContext * ctx);
extern void pp_parser_parse(ParserContext * ctx);
extern void pp_parser_print_ast(
	ParserContext * ctx,
	FILE *file
);

#endif
