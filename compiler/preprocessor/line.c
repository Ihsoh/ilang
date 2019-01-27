#include "line.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "../../string.h"
#include "../../map.h"
#include "../../lexer.h"
#include "../../parser.h"
#include "../../util.h"

#include "parser.h"
#include "gen.h"




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




#define	_TOKEN_SPACE					0x0000
#define	_TOKEN_LITERAL_CHAR				0x0001
#define	_TOKEN_LITERAL_STRING			0x0002

#define	_TOKEN_PNCT						0x0010
#define	_TOKEN_PNCT_DOUBLE_POUND		0x0011
#define	_TOKEN_PNCT_POUND				0x0012

#define	_TOKEN_IDENTIFIER				0x0020
#define	_TOKEN_FUNC_MACRO_CALL			0x0021
#define	_TOKEN_TEXT						0x0022	

/*
	捕获空白字符。
*/
LEXER_MATCHER(space)
	*abandon = 0;

	if (isspace(lexer_next_char(ctx))) {
		return lexer_new_token(ctx, _TOKEN_SPACE);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获字符字面量。
*/
LEXER_MATCHER(literal_char)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“'”。
	if (chr == '\'') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_CHAR);

		// 捕获了一个不是“'”的字符。
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\'', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		// 如果“'”之后的第一个字符是“\”，则继续捕获一个字符。
		if (chr == '\\') {
			LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_char_esc(chr), {
				// MATCHED
			}, {
				// NOT MATCHED
				return NULL;
			})
		} else {
			if (!util_is_literal_char_item(chr)) {
				return NULL;
			}
		}

		// 捕获“'”，字符字面量必须以“'”结束。
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '\'', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		return lexer_clone_token(ctx, &token);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获字符串字面量。
*/
LEXER_MATCHER(literal_string)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“"”。
	if (chr == '"') {
		lexer_init_token(ctx, &token, _TOKEN_LITERAL_STRING);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_string_item(chr) && chr != '"' && chr != -1, {
				// MATCHED
				if (chr == '\\') {
					LEXER_MATCH_CHAR(ctx, &chr, &token, util_is_literal_string_esc(chr), {
						// MATCHED
					}, {
						// NOT MATCHED
						return NULL;
					})
				}
			}, {
				// NOT MATCHED
				if (chr == -1) {
					return NULL;
				}

				lexer_next_char(ctx);
				token.len++;
				return lexer_clone_token(ctx, &token);
			})
		}
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获“##”。
*/
LEXER_MATCHER(pnct_double_pound)
	*abandon = 1;

	LexerToken token;

	char chr = lexer_next_char(ctx);

	if (chr == '#') {
		lexer_init_token(ctx, &token, _TOKEN_PNCT_DOUBLE_POUND);

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '#', {
			// MATCHED
			return lexer_clone_token(ctx, &token);
		}, {
			// NOT MATCHED
			return NULL;
		})
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获“#”。
*/
LEXER_MATCHER(pnct_pound)
	*abandon = 0;

	if (lexer_next_char(ctx) == '#') {
		return lexer_new_token(ctx, _TOKEN_PNCT_POUND);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获标点符号字符。
*/
LEXER_MATCHER(pnct)
	*abandon = 0;

	char chr = lexer_next_char(ctx);

	if (ispunct(chr) && chr != '_') {
		return lexer_new_token(ctx, _TOKEN_PNCT);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获函数宏调用。

	函数宏调用Token以一个标识符开头，该标识符必须被定义为函数宏（#define ID(...) xxxx），
	其后跟随一个“(”，并以“)”结束，类似：
		ID()
		ID(a)
		ID(a, b)
*/
LEXER_MATCHER(func_macro_call)
	*abandon = 0;

	// 
	assert(ctx->data);
	PPGeneratorContext *genctx = (PPGeneratorContext *) ctx->data;

	LexerToken token;

	char chr = lexer_next_char(ctx);

	if (isalpha(chr) || chr == '_') {
		lexer_init_token(ctx, &token, _TOKEN_FUNC_MACRO_CALL);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, isalnum(chr) || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				break;
			})
		}

		ResizableString rstr_id;
		rstr_init_with_raw(&rstr_id, token.content, token.len);
		if (!map_exists_key(genctx->map_kv_with_params, &rstr_id)) {
			rstr_free(&rstr_id);
			return NULL;
		}
		rstr_free(&rstr_id);

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '(', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		int paren_left_count = 1;
		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != ')', {
				// MATCHED
				if (chr == '(') {
					++paren_left_count;
				}
			}, {
				// NOT MATCHED
				if (paren_left_count > 1) {
					--paren_left_count;
					LEXER_NEXT_CHAR(ctx, &token)
				} else if (paren_left_count == 1) {
					LEXER_NEXT_CHAR(ctx, &token)
					break;
				} else {
					assert(0);
					break;
				}
			})
		}

		return lexer_clone_token(ctx, &token);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	捕获标识符。
*/
LEXER_MATCHER(identifier)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);

	if (isalpha(chr) || chr == '_') {
		lexer_init_token(ctx, &token, _TOKEN_IDENTIFIER);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, isalnum(chr) || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				break;
			})
		}

		return lexer_clone_token(ctx, &token);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

/*
	普通文本。
*/
LEXER_MATCHER(text)
	*abandon = 0;

	LexerToken token;

	lexer_next_char(ctx);

	lexer_init_token(ctx, &token, _TOKEN_TEXT);

	char chr;
	for (;;) {
		LEXER_MATCH_CHAR(ctx, &chr, &token, !isspace(chr) && !ispunct(chr) && chr != -1, {
			// MATCHED
		}, {
			// NOT MATCHED
			return lexer_clone_token(ctx, &token);
		})
	}
LEXER_MATCHER_END

#define	_MATCHER_LIST_ITEM(name)	{LEXER_MATCHER_NAME_S(name), LEXER_MATCHER_NAME(name)}

static LexerTokenMatcher _matchers[] = {
	_MATCHER_LIST_ITEM(space),
	_MATCHER_LIST_ITEM(literal_char),
	_MATCHER_LIST_ITEM(literal_string),

	_MATCHER_LIST_ITEM(pnct_double_pound),
	_MATCHER_LIST_ITEM(pnct_pound),
	_MATCHER_LIST_ITEM(pnct),
	
	_MATCHER_LIST_ITEM(func_macro_call),
	_MATCHER_LIST_ITEM(identifier),
	_MATCHER_LIST_ITEM(text),

	{NULL, NULL}
};

static void _expand_rstr(
	ResizableString *rstr,
	PPGeneratorContext *genctx,
	const char *src,
	int len
);

static void _expand_with_params_rstr(
	ResizableString *rstr,
	PPGeneratorContext *genctx,
	const char *src,
	int len,
	Map *params
);

static void _expand_macro_rstr(
	ResizableString *rstr,
	PPGeneratorContext *genctx,
	const char *key,
	int len
) {
	assert(rstr);
	assert(genctx);
	assert(key);

	ResizableString rstr_key;
	rstr_init_with_raw(&rstr_key, key, len);

	if (map_exists_key(genctx->map_kv, &rstr_key)) {
		ResizableString *value = map_get(genctx->map_kv, &rstr_key);
		_expand_rstr(rstr, genctx, RSTR_CSTR(value), RSTR_LEN(value));
	} else {
		rstr_append_with_raw(rstr, key, len);
	}

	rstr_free(&rstr_key);
}

static void _expand_rstr(
	ResizableString *rstr,
	PPGeneratorContext *genctx,
	const char *src,
	int len
) {
	_expand_with_params_rstr(
		rstr, genctx, src, len, NULL
	);
}

static void _expand_with_params_rstr(
	ResizableString *rstr,
	PPGeneratorContext *genctx,
	const char *src,
	int len,
	Map *params
) {
	assert(rstr);
	assert(genctx);
	assert(src);

	// 创建一个Lexer用于解析普通行。
	// 这些普通行内包含一些宏调用，需要把它们识别出来，并在后续将它们展开。
	LexerContext *ctx = lexer_new_context(
		genctx->psrctx->lexctx->file,
		src,
		len,
		_matchers
	);

	// 需要把PPGeneratorContext的实例传递给Lexer。
	//
	// 其中一个作用是被用于LEXER_MATCHER(func_macro_call)中，用于判断一个标识符
	// 是否已经被定义为函数宏。
	ctx->data = genctx;

	lexer_parse_first(ctx);

	LexerToken *token;
	while ((token = lexer_next_token(ctx)) != NULL) {
		switch (token->type) {
			case _TOKEN_SPACE:
			case _TOKEN_LITERAL_CHAR:
			case _TOKEN_LITERAL_STRING:
			case _TOKEN_PNCT:
			case _TOKEN_TEXT: {
				// 如果为以下的任意一项，将直接原样疏忽：
				//		1、空白
				//		2、字符常量
				//		3、字符串常量
				//		4、普通标点符号
				//		5、普通文本（非##、#、函数宏调用、标识符）
				rstr_append_with_raw(rstr, token->content, token->len);

				break;
			}
			case _TOKEN_PNCT_POUND: {
				// 
				ResizableString rstr_id;
				LexerToken *tk_id = lexer_next_token(ctx);
				if (tk_id == NULL
						|| tk_id->type != _TOKEN_IDENTIFIER) {
					_SYNERR_TOKEN(genctx, token, "expect identifier.");
				}
				rstr_init_with_raw(&rstr_id, tk_id->content, tk_id->len);

				rstr_append_with_char(rstr, '\"');

				if (params != NULL
						&& map_exists_key(params, &rstr_id)) {
					ResizableString *rstr_val = map_get(params, &rstr_id);
					rstr_append_with_rstr(rstr, rstr_val);
				} else {
					_expand_macro_rstr(rstr, genctx, tk_id->content, tk_id->len);
				}
				
				rstr_append_with_char(rstr, '\"');
				break;
			}
			case _TOKEN_IDENTIFIER: {
				ResizableString rstr_id;
				rstr_init_with_raw(&rstr_id, token->content, token->len);

				if (params != NULL
						&& map_exists_key(params, &rstr_id)) {
					ResizableString *rstr_val = map_get(params, &rstr_id);
					rstr_append_with_rstr(rstr, rstr_val);
				} else {
					_expand_macro_rstr(rstr, genctx, token->content, token->len);
				}

				rstr_free(&rstr_id);
				break;
			}
			case _TOKEN_FUNC_MACRO_CALL: {
				ResizableString rstr_id;
				rstr_init(&rstr_id);

				ResizableString rstr_param_values;
				rstr_init(&rstr_param_values);

				int idx = 0;
				for (; idx < token->len; idx++) {
					if (token->content[idx] != '(') {
						rstr_append_with_char(&rstr_id, token->content[idx]);
					} else {
						break;
					}
				}

				for (; idx < token->len; idx++) {
					rstr_append_with_char(&rstr_param_values, token->content[idx]);
				}
			
				PPGeneratorFuncMacro *macro = map_get(genctx->map_kv_with_params, &rstr_id);

				Map map_params;
				map_init(
					&map_params,
					map_rstr_comparer, map_rstr_comparer,
					(MapReleaser) rstr_delete, (MapReleaser) rstr_delete
				);

				LinkedList llst_param_values;
				llst_init(
					&llst_param_values, NULL, (LinkedListReleaser) rstr_delete
				);
				
				
				int params_idx = 1;	// 跳过'('。
				#define	_GET_PARAMS_CHAR(i)	((i) >= RSTR_LEN(&rstr_param_values) ? -1 : RSTR_CHAR(&rstr_param_values, i))

				int paren_left_count = 1;
				ResizableString *rstr_param = rstr_new();
				while (_GET_PARAMS_CHAR(params_idx) != -1) {
					if (_GET_PARAMS_CHAR(params_idx) == ',') {
						if (paren_left_count > 1) {
							rstr_append_with_char(rstr_param, _GET_PARAMS_CHAR(params_idx));
						} else if (paren_left_count == 1) {
							if (RSTR_LEN(rstr_param) == 0) {
								_SYNERR_TOKEN(genctx, token, "macro call parameter cannot be empty.");
							}
							llst_append(&llst_param_values, rstr_param);
							rstr_param = rstr_new();
						} else {
							assert(0);
							break;
						}
					} else if (_GET_PARAMS_CHAR(params_idx) == '(') {
						rstr_append_with_char(rstr_param, _GET_PARAMS_CHAR(params_idx));
						++paren_left_count;
					} else if (_GET_PARAMS_CHAR(params_idx) == ')') {
						if (paren_left_count > 1) {
							--paren_left_count;
							rstr_append_with_char(rstr_param, _GET_PARAMS_CHAR(params_idx));
						} else if (paren_left_count == 1) {
							if (RSTR_LEN(rstr_param) > 0) {
								llst_append(&llst_param_values, rstr_param);
								rstr_param = NULL;
							} else {
								if (LLST_LEN(&llst_param_values) != 0) {
									_SYNERR_TOKEN(genctx, token, "macro call parameter cannot be empty.");
								}
							}
							break;
						} else {
							assert(0);
							break;
						}
					} else {
						rstr_append_with_char(rstr_param, _GET_PARAMS_CHAR(params_idx));
					}

					++params_idx;
				}
				if (rstr_param != NULL) {
					rstr_delete(rstr_param);
					rstr_param = NULL;
				}

				#undef	_GET_PARAMS_CHAR


				if (macro->params.count != llst_param_values.count) {
					_SYNERR_TOKEN(genctx, token, "function macro parameter count not matched.");
				}

				LinkedListNode *node_param_name = macro->params.head;
				LinkedListNode *node_param_value = llst_param_values.head;
				while (node_param_name != NULL) {
					ResizableString *rstr_param_name = (ResizableString *) node_param_name->data;
					ResizableString *rstr_param_value = (ResizableString *) node_param_value->data;

					ResizableString *rstr_expanded_param_value = rstr_new();

					ResizableString rstr_tmp;
					rstr_init(&rstr_tmp);
					rstr_trim_ex(&rstr_tmp, rstr_param_value, RSTR_SPACE_CHAR_COMPARER);

					_expand_with_params_rstr(
						rstr_expanded_param_value,
						genctx,
						RSTR_CSTR(&rstr_tmp),
						RSTR_LEN(&rstr_tmp),
						params
					);

					rstr_free(&rstr_tmp);

					map_set(
						&map_params,
						rstr_param_name,
						rstr_expanded_param_value,
						true
					);

					node_param_name = node_param_name->next;
					node_param_value = node_param_value->next;
				}

				_expand_with_params_rstr(
					rstr, genctx, RSTR_CSTR(&(macro->body)), RSTR_LEN(&(macro->body)), &map_params
				);

				llst_free(&llst_param_values, true);
				
				map_free(&map_params, false, true);

				rstr_free(&rstr_id);

				rstr_free(&rstr_param_values);

				break;
			}
			default: {
				assert(0);
			}
		}
	}

	lexer_free_context(ctx);
}

static void _expand(
	PPGeneratorContext *genctx,
	const char *src,
	int len
) {
	assert(genctx);
	assert(src);

	ResizableString rstr;
	rstr_init(&rstr);

	_expand_rstr(&rstr, genctx, src, len);
	_OUT_STR(genctx, RSTR_CSTR(&rstr), RSTR_LEN(&rstr));

	rstr_free(&rstr);
}

void expand_macro_rstr(
	ResizableString *rstr,
	PPGeneratorContext *genctx,
	const char *key,
	int len
) {
	assert(rstr);
	assert(genctx);
	assert(key);

	_expand_macro_rstr(rstr, genctx, key, len);
}

void pp_line_process(
	PPGeneratorContext *genctx,
	ParserASTNode *node
) {
	assert(genctx);
	assert(node);
	assert(node->type == PP_NODE_LINE);

	_expand(genctx, node->token->content, node->token->len);
}
