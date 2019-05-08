#include "gen.h"

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"
#include "../../map.h"
#include "../../list.h"
#include "../../osdep.h"

#include "preprocessor.h"
#include "comment.h"
#include "line_concat.h"
#include "parser.h"
#include "lexer.h"
#include "line.h"
#include "evaluator.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>



#define	_SYNERR(ctx, msg)	{ (ctx)->psrctx->syntax_error_msg((ctx)->psrctx, (msg)); }
#define	_SYNERR_TOKEN(ctx, token, msg)	{ (ctx)->psrctx->syntax_error_token_msg((ctx)->psrctx, (token), (msg)); }
#define	_SYNERR_NODE(ctx, node, msg, args...)	{ char __buffer__[1024]; sprintf(__buffer__, msg, ##args); (ctx)->psrctx->syntax_error_node_msg((ctx)->psrctx, (node), __buffer__); }

#define	_OUT_CHAR(ctx, chr)	\
	{	\
		if ((ctx)->out != NULL) {	\
			fputc((chr), (ctx)->out);	\
		} else {	\
			rstr_append_with_char((ctx)->result, chr);	\
		}	\
	}
#define	_OUT_STR(ctx, str, len) { for (int __i = 0; __i < (len); __i++) { _OUT_CHAR((ctx), (str)[__i]) } }
#define	_OUT_CSTR(ctx, str) { _OUT_STR((ctx), (str), strlen((str))) }
#define	_OUT_FMT(ctx, fmt, ...)	\
	{	\
		if ((ctx)->out != NULL) {	\
			fprintf((ctx)->out, fmt, __VA_ARGS__);	\
		} else {	\
			char __buffer__[1024];	\
			snprintf(__buffer__, sizeof(__buffer__), fmt, __VA_ARGS__);	\
			rstr_append_with_cstr((ctx)->result, __buffer__);	\
		}	\
	}




static void _gen_block(
	PPGeneratorContext *ctx,
	ParserASTNode *node
);

static void _gen_if_block(
	PPGeneratorContext *ctx,
	ParserASTNode *node
);




static void _gen_space(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_SPACE);

	_OUT_STR(ctx, node->token->content, node->token->len);
}

static bool _include_file(
	PPGeneratorContext *ctx,
	const char *filepath
) {
	assert(ctx);
	assert(filepath);

	ResizableString path;
 
	if (filepath[0] == '/' || filepath[0] == '\\') {
		rstr_init_with_cstr(&path, filepath);
	} else {
		rstr_init_with_cstr(&path, ctx->psrctx->lexctx->file);
		int sep_idx;
		if ((sep_idx = rstr_last_index_of(&path, "/")) != -1
				|| (sep_idx = rstr_last_index_of(&path, "\\")) != -1) {
			ResizableString tmp;
			rstr_init(&tmp);
			rstr_sub_string(&tmp, &path, 0, sep_idx);
			rstr_append_with_cstr(&tmp, OSDEP_PATH_SEPERATOR);
			rstr_append_with_cstr(&tmp, filepath);
			rstr_clear(&path);
			rstr_append_with_rstr(&path, &tmp);
			rstr_free(&tmp);
		} else {
			rstr_clear(&path);
			rstr_append_with_cstr(&path, filepath);
		}
	}

	const char *source = NULL;
	int len = 0;
	source = (const char *) util_get_file_content(RSTR_CSTR(&path), &len);
	if (source == NULL) {
		goto error;
	}

	ResizableString rstr_comment_removed;
	rstr_init(&rstr_comment_removed);
	pp_comment_process(
		&rstr_comment_removed,
		RSTR_CSTR(&path),
		source,
		len
	);

	ResizableString rstr_line_concated;
	rstr_init(&rstr_line_concated);
	pp_line_concat_process(
		&rstr_line_concated,
		RSTR_CSTR(&path),
		RSTR_CSTR(&rstr_comment_removed),
		RSTR_LEN(&rstr_comment_removed)
	);

	rstr_free(&rstr_comment_removed);

	ParserContext *ctx_psr = pp_parser_new_context(
		RSTR_CSTR(&path), RSTR_CSTR(&rstr_line_concated), RSTR_LEN(&rstr_line_concated)
	);
	pp_parser_parse(ctx_psr);

	PPGeneratorContext *pp_gen_ctx = pp_gen_new_context(
		ctx_psr,
		ctx->cstr_include_paths,
		ctx->map_kv,
		ctx->map_kv_with_params,
		ctx->out,
		ctx->result
	);
	pp_gen_generate(pp_gen_ctx);

	// include时使用的map_kv和map_kv_with_params是从包含#include指令的ctx来的。
	// 不能在include时新建的ctx中释放掉。
	pp_gen_ctx->map_kv = NULL;
	pp_gen_ctx->map_kv_with_params = NULL;
	pp_gen_free_context(pp_gen_ctx);

	pp_parser_free_context(ctx_psr);

	free((void *) source);

	rstr_free(&path);
	rstr_free(&rstr_line_concated);
	return true;

error:

	rstr_free(&path);
	rstr_free(&rstr_line_concated);
	return false;
}

static void _include_file_from_include_path(
	PPGeneratorContext *ctx,
	ParserASTNode *node,
	ResizableString *filename
) {
	assert(ctx);
	assert(node);
	assert(filename);

	bool included = false;
	LinkedListNode *n = ctx->include_paths.head;
	while (!included && n != NULL) {
		ResizableString filepath;
		rstr_init_with_rstr(&filepath, n->data);
		rstr_append_with_rstr(&filepath, filename);

		if (_include_file(ctx, RSTR_CSTR(&filepath))) {
			included = true;
		}

		rstr_free(&filepath);
		n = n->next;
	}
	if (!included) {
		_SYNERR_NODE(ctx, node, "cannot include file.");
	}
}

static void _gen_include(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_INCLUDE);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#include";
	ResizableString filename;
	rstr_init(&filename);
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	if (_GET_CHAR(idx) == '<') {
		idx++;
		while (_GET_CHAR(idx) != '>' && _GET_CHAR(idx) != -1) {
			rstr_append_with_char(&filename, _GET_CHAR(idx));
			idx++;
		}
		if (_GET_CHAR(idx) != '>') {
			_SYNERR_NODE(ctx, node, "expect '>'.");
		}

		_OUT_FMT(ctx, "#line %d \"%s\"\n", 1, RSTR_CSTR(&filename));
		_include_file_from_include_path(ctx, node, &filename);
		_OUT_FMT(ctx, "#line %d \"%s\"", node->line + 1, node->file);
	} else if (_GET_CHAR(idx) == '"') {
		idx++;
		while (_GET_CHAR(idx) != '"' && _GET_CHAR(idx) != -1) {
			rstr_append_with_char(&filename, _GET_CHAR(idx));
			idx++;
		}
		if (_GET_CHAR(idx) != '"') {
			_SYNERR_NODE(ctx, node, "expect '\"'.");
		}

		_OUT_FMT(ctx, "#line %d \"%s\"\n", 1, RSTR_CSTR(&filename));
		if (!_include_file(ctx, RSTR_CSTR(&filename))) {
			_include_file_from_include_path(ctx, node, &filename);
		}
		_OUT_FMT(ctx, "#line %d \"%s\"", node->line + 1, node->file);
	} else {
		_SYNERR_NODE(ctx, node, "expect '<' or '\"'.");
	}

	rstr_free(&filename);

	#undef	_GET_CHAR
}

static PPGeneratorFuncMacro * _new_gen_func_macro(void) {
	PPGeneratorFuncMacro *macro = (PPGeneratorFuncMacro *) malloc(sizeof(PPGeneratorFuncMacro));
	llst_init(
		&(macro->params),
		NULL,
		(LinkedListReleaser) rstr_delete
	);
	rstr_init(&(macro->body));
	return macro;
}

static void _delete_gen_func_macro(PPGeneratorFuncMacro *macro) {
	assert(macro);
	llst_free(&(macro->params), true);
	rstr_free(&(macro->body));
	free(macro);
}

static bool _is_macro_defined(
	PPGeneratorContext *ctx,
	ResizableString *key
) {
	assert(ctx);
	assert(key);

	return map_exists_key(ctx->map_kv, key)
		|| map_exists_key(ctx->map_kv_with_params, key);
}

static void _delete_macro(
	PPGeneratorContext *ctx,
	ResizableString *key
) {
	assert(ctx);
	assert(key);

	if (map_exists_key(ctx->map_kv, key)) {
		map_remove(ctx->map_kv, key, true, true);
	} else if (map_exists_key(ctx->map_kv_with_params, key)) {
		map_remove(ctx->map_kv_with_params, key, true, true);
	}
}

static void _gen_define(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_DEFINE);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#define";
	ResizableString *key = rstr_new();
	ResizableString *value = rstr_new();
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])
	#define	_IGNORE_SPACE	\
		while (isspace(_GET_CHAR(idx))) {	\
			idx++;	\
		}

	_IGNORE_SPACE

	while (_GET_CHAR(idx) != -1 && util_is_identifier_item(_GET_CHAR(idx))) {
		rstr_append_with_char(key, _GET_CHAR(idx));
		idx++;
	}

	if (RSTR_LEN(key) == 0) {
		_SYNERR_NODE(ctx, node, "expect key.");
	}

	if (_is_macro_defined(ctx, key)) {
		_SYNERR_NODE(ctx, node, "macro is defined.");
	}

	if (_GET_CHAR(idx) == '(') {
		idx++;

		PPGeneratorFuncMacro *macro = _new_gen_func_macro();

		for (;;) {
			_IGNORE_SPACE

			if (_GET_CHAR(idx) == ')') {
				idx++;
				break;
			}

			ResizableString *param = rstr_new();
			while (util_is_identifier_item(_GET_CHAR(idx))) {
				rstr_append_with_char(param, _GET_CHAR(idx));
				idx++;
			}
			if (RSTR_LEN(param) == 0) {
				_SYNERR_NODE(ctx, node, "expect identifier.");
			}
			llst_append(&(macro->params), param);

			_IGNORE_SPACE

			if (_GET_CHAR(idx) == ')') {
				idx++;
				break;
			}

			if (_GET_CHAR(idx) == ',') {
				idx++;
			} else {
				_SYNERR_NODE(ctx, node, "expect ','.");
			}
		}

		_IGNORE_SPACE

		while (_GET_CHAR(idx) != -1) {
			rstr_append_with_char(&(macro->body), _GET_CHAR(idx));
			idx++;
		}

		map_set(ctx->map_kv_with_params, key, macro, true);
	} else {
		_IGNORE_SPACE

		while (_GET_CHAR(idx) != -1) {
			rstr_append_with_char(value, _GET_CHAR(idx));
			idx++;
		}

		map_set(ctx->map_kv, key, value, true);
	}

	#undef	_GET_CHAR
}

static void _gen_undef(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_UNDEF);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#undef";
	ResizableString key;
	rstr_init(&key);
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1 && !isspace(_GET_CHAR(idx))) {
		rstr_append_with_char(&key, _GET_CHAR(idx));
		idx++;
	}

	if (RSTR_LEN(&key) == 0) {
		_SYNERR_NODE(ctx, node, "expect key.");
	}

	_delete_macro(ctx, &key);

	rstr_free(&key);

	#undef	_GET_CHAR
}

static bool _gen_elif(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_ELIF);
	assert(node->nchilds == 1);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#elif";
	ResizableString temp;
	rstr_init(&temp);
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1) {
		rstr_append_with_char(&temp, _GET_CHAR(idx));
		idx++;
	}

	ResizableString expr;
	rstr_init(&expr);

	rstr_rtrim_ex(&expr, &temp, RSTR_SPACE_CHAR_COMPARER);

	if (RSTR_LEN(&expr) == 0) {
		_SYNERR_NODE(ctx, node, "expect constant expression.");
	}

	bool cond = pp_eval(ctx, RSTR_CSTR(&expr), RSTR_LEN(&expr)) != 0;

	if (cond) {
		ParserASTNode *node_block = node->childs[0];
		assert(node_block->type == PP_NODE_BLOCK);

		_gen_if_block(ctx, node_block);
	}

	rstr_free(&expr);

	rstr_free(&temp);

	#undef	_GET_CHAR

	return cond;
}

static bool _gen_elifxdef(
	PPGeneratorContext *ctx,
	ParserASTNode *node,
	bool not
) {
	assert(ctx);
	assert(node);
	if (not) {
		assert(node->type == PP_NODE_DIRECTIVE_ELIFNDEF);
	} else {
		assert(node->type == PP_NODE_DIRECTIVE_ELIFDEF);
	}
	assert(node->nchilds == 1);

	const char *content = node->token->content;
	int len = node->token->len;

	ResizableString key;
	rstr_init(&key);
	int idx;
	if (not) {
		char cmd[] = "#elifndef";
		idx = sizeof(cmd);
	} else {
		char cmd[] = "#elifdef";
		idx = sizeof(cmd);
	}

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1 && !isspace(_GET_CHAR(idx))) {
		rstr_append_with_char(&key, _GET_CHAR(idx));
		idx++;
	}

	if (RSTR_LEN(&key) == 0) {
		_SYNERR_NODE(ctx, node, "expect key.");
	}

	bool cond;
	if (not) {
		cond = !map_exists_key(ctx->map_kv, &key);
	} else {
		cond = map_exists_key(ctx->map_kv, &key);
	}

	if (cond) {
		ParserASTNode *node_block = node->childs[0];
		assert(node_block->type == PP_NODE_BLOCK);

		_gen_if_block(ctx, node_block);
	}

	rstr_free(&key);

	#undef	_GET_CHAR

	return cond;
}

static bool _gen_elifxxx(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case PP_NODE_DIRECTIVE_ELIF: {
			return _gen_elif(ctx, node);
		}
		case PP_NODE_DIRECTIVE_ELIFDEF: {
			return _gen_elifxdef(ctx, node, false);
		}
		case PP_NODE_DIRECTIVE_ELIFNDEF: {
			return _gen_elifxdef(ctx, node, true);
		}
		default: {
			assert(0);
		}
	}
}

static void _gen_else(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_ELSE);
	assert(node->nchilds == 1);

	ParserASTNode *node_block = node->childs[0];
	assert(node_block->type == PP_NODE_BLOCK);

	_gen_if_block(ctx, node_block);
}

static bool _gen_elifxxx_and_else(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case PP_NODE_DIRECTIVE_ELIF:
		case PP_NODE_DIRECTIVE_ELIFDEF:
		case PP_NODE_DIRECTIVE_ELIFNDEF: {
			return _gen_elifxxx(ctx, node);
		}
		case PP_NODE_DIRECTIVE_ELSE: {
			_gen_else(ctx, node);
			return true;
		}
		default: {
			assert(0);
			return false;
		}
	}
}

static void _gen_if(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_IF);
	assert(node->nchilds >= 1);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#if";
	ResizableString temp;
	rstr_init(&temp);
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1) {
		rstr_append_with_char(&temp, _GET_CHAR(idx));
		idx++;
	}

	ResizableString expr;
	rstr_init(&expr);

	rstr_rtrim_ex(&expr, &temp, RSTR_SPACE_CHAR_COMPARER);

	if (RSTR_LEN(&expr) == 0) {
		_SYNERR_NODE(ctx, node, "expect constant expression.");
	}

	int result = pp_eval(ctx, RSTR_CSTR(&expr), RSTR_LEN(&expr));
	
	if (result) {
		ParserASTNode *node_block = node->childs[0];
		assert(node_block->type == PP_NODE_BLOCK);

		_gen_if_block(ctx, node_block);
	} else {
		for (int i = 1; i < node->nchilds; i++) {
			ParserASTNode *child = node->childs[i];
			if (_gen_elifxxx_and_else(ctx, child)) {
				break;
			}
		}
	}

	LexerToken *token_endif = PP_IFXXX_AST_NODE_GET_TOKEN_ENDIF(node);
	assert(token_endif);
	_OUT_FMT(ctx, "#line %d \"%s\"", token_endif->line + 1, token_endif->file);

	rstr_free(&expr);

	rstr_free(&temp);

	#undef	_GET_CHAR
}

static void _gen_ifxdef(
	PPGeneratorContext *ctx,
	ParserASTNode *node,
	bool not
) {
	assert(ctx);
	assert(node);
	if (not) {
		assert(node->type == PP_NODE_DIRECTIVE_IFNDEF);
	} else {
		assert(node->type == PP_NODE_DIRECTIVE_IFDEF);
	}
	assert(node->nchilds >= 1);

	const char *content = node->token->content;
	int len = node->token->len;

	ResizableString key;
	rstr_init(&key);
	int idx;
	if (not) {
		char cmd[] = "#ifndef";
		idx = sizeof(cmd);
	} else {
		char cmd[] = "#ifdef";
		idx = sizeof(cmd);
	}

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1 && !isspace(_GET_CHAR(idx))) {
		rstr_append_with_char(&key, _GET_CHAR(idx));
		idx++;
	}

	if (RSTR_LEN(&key) == 0) {
		_SYNERR_NODE(ctx, node, "expect key.");
	}

	bool cond;
	if (not) {
		cond = !map_exists_key(ctx->map_kv, &key);
	} else {
		cond = map_exists_key(ctx->map_kv, &key);
	}

	if (cond) {
		ParserASTNode *node_block = node->childs[0];
		assert(node_block->type == PP_NODE_BLOCK);

		_gen_if_block(ctx, node_block);
	} else {
		for (int i = 1; i < node->nchilds; i++) {
			ParserASTNode *child = node->childs[i];
			if (_gen_elifxxx_and_else(ctx, child)) {
				break;
			}
		}
	}

	rstr_free(&key);

	#undef	_GET_CHAR
}

static void _gen_ifdef(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_IFDEF);
	assert(node->nchilds >= 1);

	_gen_ifxdef(ctx, node, false);

	LexerToken *token_endif = PP_IFXXX_AST_NODE_GET_TOKEN_ENDIF(node);
	assert(token_endif);
	_OUT_FMT(ctx, "#line %d \"%s\"", token_endif->line + 1, token_endif->file);
}

static void _gen_ifndef(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_IFNDEF);
	assert(node->nchilds >= 1);

	_gen_ifxdef(ctx, node, true);

	LexerToken *token_endif = PP_IFXXX_AST_NODE_GET_TOKEN_ENDIF(node);
	assert(token_endif);
	_OUT_FMT(ctx, "#line %d \"%s\"", token_endif->line + 1, token_endif->file);
}

static void _gen_info(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_INFO);
	assert(node->nchilds == 0);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#info";
	ResizableString temp;
	rstr_init(&temp);
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1) {
		rstr_append_with_char(&temp, _GET_CHAR(idx));
		idx++;
	}

	fprintf(stdout, "%s\n", RSTR_CSTR(&temp));

	rstr_free(&temp);

	#undef	_GET_CHAR
}

static void _gen_warn(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_WARN);
	assert(node->nchilds == 0);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#warn";
	ResizableString temp;
	rstr_init(&temp);
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1) {
		rstr_append_with_char(&temp, _GET_CHAR(idx));
		idx++;
	}

	fprintf(stdout, "%s\n", RSTR_CSTR(&temp));

	rstr_free(&temp);

	#undef	_GET_CHAR
}

static void _gen_error(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_DIRECTIVE_ERROR);
	assert(node->nchilds == 0);

	const char *content = node->token->content;
	int len = node->token->len;

	char cmd[] = "#error";
	ResizableString temp;
	rstr_init(&temp);
	int idx = sizeof(cmd);

	#define	_GET_CHAR(i) ((i) >= len ? -1 : content[(i)])

	while (isspace(_GET_CHAR(idx))) {
		idx++;
	}

	while (_GET_CHAR(idx) != -1) {
		rstr_append_with_char(&temp, _GET_CHAR(idx));
		idx++;
	}

	fprintf(stderr, "%s\n", RSTR_CSTR(&temp));

	rstr_free(&temp);

	exit(PP_DIRECTIVE_ERROR_EXIT_CODE);

	#undef	_GET_CHAR
}

static void _gen_cmd_line(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_CMD_LINE);

	_OUT_STR(ctx, node->token->content, node->token->len);
}

static void _gen_line(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_LINE);

	pp_line_process(ctx, node);
}

static void _gen_block_item(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	ResizableString *rstr_file = rstr_new();
	rstr_appendf(rstr_file, "\"%s\"", node->file);
	pp_gen_define(
		ctx,
		rstr_new_with_cstr("__FILE__"),
		rstr_file
	);

	ResizableString *rstr_line = rstr_new();
	rstr_appendf(rstr_line, "%d", node->line);
	pp_gen_define(
		ctx,
		rstr_new_with_cstr("__LINE__"),
		rstr_line
	);

	switch (node->type) {
		case PP_NODE_SPACE: {
			_gen_space(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_INCLUDE: {
			_gen_include(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_DEFINE: {
			_gen_define(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_UNDEF: {
			_gen_undef(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_IF: {
			_gen_if(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_IFDEF: {
			_gen_ifdef(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_IFNDEF: {
			_gen_ifndef(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_INFO: {
			_gen_info(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_WARN: {
			_gen_warn(ctx, node);
			break;
		}
		case PP_NODE_DIRECTIVE_ERROR: {
			_gen_error(ctx, node);
			break;
		}
		case PP_NODE_CMD_LINE: {
			_gen_cmd_line(ctx, node);
			break;
		}
		case PP_NODE_LINE: {
			_gen_line(ctx, node);
			break;
		}
		default: {
			assert(0);
		}
	}
}

static void _gen_block(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_BLOCK);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *child = node->childs[i];
		_gen_block_item(ctx, child);
	}
}

static void _gen_if_block(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_BLOCK);

	if (node->nchilds > 0) {
		ParserASTNode *child = node->childs[0];
		_OUT_FMT(ctx, "#line %d \"%s\"", child->line, child->file);
	}

	_gen_block(ctx, node);
}

static void _gen_module(
	PPGeneratorContext *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == PP_NODE_MODULE);

	_gen_block(ctx, node->childs[0]);
}




void pp_gen_generate(PPGeneratorContext *ctx) {
	assert(ctx);
	assert(ctx->psrctx);
	assert(ctx->psrctx->ast);
	assert(ctx->psrctx->ast->type == PP_NODE_MODULE);

	_gen_module(ctx, ctx->psrctx->ast);
}




PPGeneratorContext * pp_gen_new_context(
	ParserContext *psrctx,
	const char *include_paths,
	Map *map_kv,
	Map *map_kv_with_params,
	FILE *out,
	ResizableString *result
) {
	assert(psrctx);
	assert(psrctx->ast);
	assert(include_paths);
	assert(out != NULL || result != NULL);

	PPGeneratorContext *ctx = (PPGeneratorContext *)malloc(sizeof(PPGeneratorContext));

	ctx->psrctx = psrctx;
	ctx->out = out;
	ctx->result = result;

	if (map_kv != NULL) {
		ctx->map_kv = map_kv;
	} else {
		ctx->map_kv = (Map *)malloc(sizeof(Map));
		assert(ctx->map_kv);
		map_init(
			ctx->map_kv,
			map_rstr_comparer, map_rstr_comparer,
			(MapReleaser) rstr_delete, (MapReleaser) rstr_delete
		);
	}

	if (map_kv_with_params != NULL) {
		ctx->map_kv_with_params = map_kv_with_params;
	} else {
		ctx->map_kv_with_params = (Map *)malloc(sizeof(Map));
		assert(ctx->map_kv_with_params);
		map_init(
			ctx->map_kv_with_params,
			map_rstr_comparer, NULL,
			(MapReleaser) rstr_delete, (MapReleaser) _delete_gen_func_macro
		);
	}

	ctx->cstr_include_paths = include_paths;

	llst_init(
		&(ctx->include_paths),
		NULL,
		(LinkedListReleaser) rstr_delete
	);

	ResizableString rstr_inc_paths;
	rstr_init_with_cstr(&rstr_inc_paths, include_paths);

	rstr_split(
		&(ctx->include_paths),
		&rstr_inc_paths,
		";",
		true
	);

	rstr_free(&rstr_inc_paths);

	return ctx;
}

void pp_gen_free_context(PPGeneratorContext *ctx) {
	assert(ctx);

	if (ctx->map_kv != NULL) {
		map_free(ctx->map_kv, true, true);
		free(ctx->map_kv);
	}

	if (ctx->map_kv_with_params != NULL) {
		map_free(ctx->map_kv_with_params, true, true);
		free(ctx->map_kv_with_params);
	}

	llst_free(&(ctx->include_paths), true);

	free(ctx);
}

void pp_gen_define(
	PPGeneratorContext *ctx,
	ResizableString *k,
	ResizableString *v
) {
	assert(ctx);
	assert(k);

	ResizableString *key = rstr_new_with_rstr(k);
	if (RSTR_LEN(key) == 0) {
		_SYNERR(ctx, "invalid macro name.");
	}
	for (int i = 0; i < RSTR_LEN(key); i++) {
		if (!util_is_identifier_item(RSTR_CHAR(key, i))) {
			_SYNERR(ctx, "invalid macro name.");
		}
	}

	ResizableString *value;
	if (v == NULL) {
		value = rstr_new();
	} else {
		value = rstr_new_with_rstr(v);
	}

	map_remove(ctx->map_kv, key, true, true);
	map_set(ctx->map_kv, key, value, true);
}
