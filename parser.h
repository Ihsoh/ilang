#ifndef __PARSER_H__
#define __PARSER_H__

#include <inttypes.h>
#include <stddef.h>

#include "lexer.h"

struct _ParserSymbol;

typedef void (* ParserSymbolReleaser)(struct _ParserSymbol *symbol);

typedef struct _ParserSymbol {
	int						type;

	LexerToken				*token;

	struct _ParserSymbol	*next;

	struct _ParserSymbol	*symtbl;

	ParserSymbolReleaser	releaser;

	size_t					szdata;
	uint8_t					data[0];
} ParserSymbol;

struct _ParserASTNode;

typedef void (* ParserASTNodeReleaser)(struct _ParserASTNode *node);

typedef struct _ParserASTNode {
	uint32_t				type;
	char					*type_name;

	LexerToken				*token;

	const char				*file;
	int						line;
	int						column;

	// next并非用于AST结构中，单纯是为了维持一个链表结构。
	// 该链表结构方便释放ctx资源时可以遍历所有节点。	
	struct _ParserASTNode	*next;

	// parent用于在AST结构中记录父节点。
	struct _ParserASTNode 	*parent;

	ParserSymbol			*symtbl;

	int						szchilds;
	int						nchilds;
	struct _ParserASTNode	**childs;

	ParserASTNodeReleaser	releaser;

	size_t					szdata;
	uint8_t					data[0];
} ParserASTNode;

typedef struct _ParserContext {
	LexerContext		*lexctx;

	ParserASTNode		*ast;

	ParserASTNode		*nodes;

	ParserASTNode		*previous_node;
	ParserASTNode		*last_node;

	LexerToken			*current_token;

	int					arch;

	void (* alloc_mem_error)(struct _ParserContext *ctx);
	void (* syntax_error)(struct _ParserContext *ctx);
	void (*syntax_error_msg)(
		struct _ParserContext *ctx,
		const char *msg
	);
	void (*syntax_error_token_msg)(
		struct _ParserContext *ctx,
		LexerToken *token,
		const char *msg
	);
	void (*syntax_error_node_msg)(
		struct _ParserContext *ctx,
		ParserASTNode *node,
		const char *msg
	);

	size_t					szdata;
	uint8_t					data[0];
} ParserContext;



extern ParserContext * parser_new_context_with_data(
	LexerContext *lexctx,
	size_t szdata,
	void *data
);
extern ParserContext * parser_new_context(
	LexerContext *lexctx
);
extern void parser_free_context(
	ParserContext * ctx
);

#define	PARSER_NEW_NODE(ctx, type, token)	(parser_new_node((ctx), (type), #type, (token), 0, NULL))

extern ParserASTNode * parser_new_node(
	ParserContext *ctx,
	int type,
	char *type_name,
	LexerToken *token,
	size_t szdata,
	void *data
);
extern void parser_free_node(
	ParserContext *ctx,
	ParserASTNode *node
);
extern void parser_insert_node(
	ParserContext *ctx,
	ParserASTNode **pos,
	ParserASTNode *node
);
extern ParserASTNode * parser_add_child(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserASTNode *child
);
extern ParserSymbol * parser_new_symbol(
	ParserContext *ctx,
	int type,
	LexerToken *token,
	size_t szdata,
	void *data
);
extern ParserSymbol * parser_add_symptr_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	ParserSymbol *symptr
);
extern ParserSymbol * parser_add_symbol_to_node(
	ParserContext *ctx,
	ParserASTNode *node,
	int type,
	LexerToken *token,
	size_t szdata,
	void *data
);
extern ParserSymbol * parser_get_symbol_from_node(
	ParserContext *ctx,
	ParserASTNode *node,
	int type,
	LexerToken *token
);
extern void parser_free_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol
);
extern ParserSymbol * parser_add_symptr_to_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	ParserSymbol *symptr
);
extern ParserSymbol * parser_add_symbol_to_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	int type,
	LexerToken *token,
	size_t szdata,
	void *data
);
extern ParserSymbol * parser_get_symbol_from_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	int type,
	LexerToken *token
);
extern int parser_get_symbol_index_from_symbol(
	ParserContext *ctx,
	ParserSymbol *symbol,
	int type,
	LexerToken *token
);

#endif
