#include "asmgen_gas64.h"

#include "../../parser.h"
#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"

#include "semantics.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define	_OUT_CHAR(ctx, chr)	{ fputc((chr), (ctx)->out); }
#define	_OUT_STR(ctx, str, len) { for (int __i = 0; __i < (len); __i++) { _OUT_CHAR((ctx), (str)[__i]) } }
#define	_OUT_CSTR(ctx, str) { _OUT_STR((ctx), (str), strlen((str))) }
#define	_OUT_FMT(ctx, fmt, ...) { fprintf((ctx)->out, fmt, __VA_ARGS__); }

static void _error(const char * msg) {
	assert(msg);
	fprintf(stderr, "ASMGen(GAS64) Error: %s\n", msg);
	exit(1);
}





static size_t _generate_func_tmp(
	ASMGeneratorGas64Context *ctx,
	ParserSymbol *func_symbol,
	ResizableString *rstr
) {
	assert(ctx);
	assert(func_symbol);
	assert(rstr);

	size_t no = BE_FUNC_SYMBOL_NEXT_NO(func_symbol);

	rstr_appendf(rstr, "FUNC_TMP.%zu", no);

	return no;
}

static size_t _next_no(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);

	return ctx->counter++;
}

static int _new_string(
	ASMGeneratorGas64Context *ctx,
	ResizableString *name,
	const char *str,
	size_t len
) {
	assert(ctx);
	assert(str);

	// .section __TEXT,__cstring,cstring_literals
	// STR.[X]
	// .asciz [STRING]

	size_t no = _next_no(ctx);

	if (name != NULL) {
		rstr_appendf(name, "STR.%zu", no);
	}
	
	ResizableString unescaped_str;
	rstr_init(&unescaped_str);

	be_lexer_unescape_string(
		ctx->psrctx->lexctx,
		&unescaped_str,
		str, len
	);
	int unescaped_str_len = RSTR_LEN(&unescaped_str);

	rstr_append_with_cstr(
		ctx->head,
		".section __TEXT,__cstring,cstring_literals\n"
	);
	rstr_appendf(
		ctx->head,
		"STR.%zu:\n",
		no
	);

	rstr_append_with_cstr(
		ctx->head,
		".asciz \""
	);
	for (int i = 0; i < unescaped_str_len; i++) {
		char chr = RSTR_CHAR(&unescaped_str, i);
		if (isprint(chr)) {
			rstr_append_with_char(ctx->head, chr);
		} else {
			rstr_appendf(ctx->head, "\\x%02X", chr);
		}
	}
	rstr_append_with_cstr(
		ctx->head,
		"\"\n\n"
	);

	rstr_free(&unescaped_str);

	return unescaped_str_len + 1;
}




static void _asm_constexpr(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr_val,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr_val);
	assert(node_constexpr);
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

	switch (BE_EXPR_AST_NODE_GET_CONSTANT_TYPE(node_constexpr)) {
		case BE_TYPE_CHAR: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_CHAR_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%d",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_INT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%lld",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_INT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT8: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT8_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT16: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT16_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT32: {
			char str[128];
			snprintf(
				str, sizeof(str), "%u",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT32_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_UINT64: {
			char str[128];
			snprintf(
				str, sizeof(str), "%llu",
				BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_UINT64_VAL(node_constexpr)
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_FLOAT: {
			char str[128];
			double val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_FLOAT_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "0x%llX",
				*(uint64_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_DOUBLE: {
			char str[128];
			double val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_DOUBLE_VAL(node_constexpr);
			snprintf(
				str, sizeof(str), "0x%llX",
				*(uint64_t *)&val
			);
			rstr_append_with_cstr(rstr_val, str);
			break;
		}
		case BE_TYPE_POINTER: {
			if (BE_EXPR_AST_NODE_GET_HAS_CONSTSTR(node_constexpr)) {
				BeParserConstexprString *conststr = BE_EXPR_AST_NODE_GET_CONSTSTR(node_constexpr);
				uint64_t ptr_val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);

				ResizableString conststr_name;
				rstr_init(&conststr_name);

				int len = _new_string(ctx, &conststr_name, conststr->str, conststr->len);

				rstr_appendf(
					rstr_val,
					"%s + %llu",
					RSTR_CSTR(&conststr_name),
					ptr_val
				);

				rstr_free(&conststr_name);
			} else {
				char str[128];
				uint64_t val = BE_EXPR_AST_NODE_GET_CONSTEXPR_RESULT_POINTER_VAL(node_constexpr);
				snprintf(
					str, sizeof(str), "0x%llX",
					val
				);
				rstr_append_with_cstr(rstr_val, str);
			}
			break;
		}
		default: {
			assert(0);
		}
	}
}

static void _asm_constexpr_initializer(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserASTNode *node_constexpr
) {
	assert(ctx);
	assert(rstr);\
	assert(node_constexpr);
	assert(BE_EXPR_AST_NODE_GET_CONSTANT(node_constexpr));

	_asm_constexpr(ctx, rstr, node_constexpr);
}

static void _asm_zero_initializer(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	switch (node_type->type) {
		case BE_NODE_TYPE_CHAR:
		case BE_NODE_TYPE_INT8:
		case BE_NODE_TYPE_UINT8:
		case BE_NODE_TYPE_INT16:
		case BE_NODE_TYPE_UINT16:
		case BE_NODE_TYPE_INT32:
		case BE_NODE_TYPE_UINT32:
		case BE_NODE_TYPE_INT64:
		case BE_NODE_TYPE_UINT64: {
			rstr_append_with_cstr(rstr, "0");
			break;
		}
		case BE_NODE_TYPE_FLOAT:
		case BE_NODE_TYPE_DOUBLE: {
			rstr_append_with_cstr(rstr, "0.0");
			break;
		}
		case BE_NODE_TYPE_STRUCT:
		case BE_NODE_TYPE_ARRAY: {
			assert(0);
			break;
		}
		case BE_NODE_TYPE_POINTER: {
			rstr_append_with_cstr(rstr, "0");
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _asm_global_var_initializer(
	ASMGeneratorGas64Context *ctx,
	ResizableString *rstr,
	ParserASTNode *node_constexpr,
	ParserASTNode *node_type
) {
	assert(ctx);
	assert(rstr);
	assert(node_type);

	if (node_constexpr != NULL) {
		_asm_constexpr_initializer(ctx, rstr, node_constexpr);
	} else {
		_asm_zero_initializer(ctx, rstr, node_type);
	}
}

static void _asm_var(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_VAR);
	assert(node->nchilds > 0);

	int align = BE_VAR_AST_NODE_GET_ALIGN(node);

	for (int i = 0; i < node->nchilds; i++) {
		ParserASTNode *node_var_item = node->childs[i];
		assert(node_var_item->type == BE_NODE_VAR_ITEM);
		assert(node_var_item->nchilds == 2 || node_var_item->nchilds == 3);

		ParserASTNode *node_identifier = node_var_item->childs[0];
		assert(node_identifier->type == BE_NODE_IDENTIFIER);
		ParserASTNode *node_type = node_var_item->childs[1];
		ParserASTNode *node_expr = NULL;

		if (node_var_item->nchilds == 3) {
			node_expr = node_var_item->childs[2];
		}

		switch (BE_VAR_AST_NODE_GET_TYPE(node)) {
			case BE_VAR_TYPE_GLOBAL: {
				// .section __DATA,__data
				// .globl [VAR_NAME]
				// .balign	[?]
				// [VAR_NAME]:
				// .[TYPE] [EXPR]

				rstr_append_with_cstr(ctx->head, ".section __DATA,__data\n");
				
				ResizableString rstr_var_name;
				rstr_init(&rstr_var_name);
				rstr_append_with_raw(
					&rstr_var_name,
					node_identifier->token->content,
					node_identifier->token->len
				);

				rstr_append_with_cstr(ctx->head, ".globl ");
				rstr_append_with_raw(ctx->head, node_identifier->token->content, node_identifier->token->len);
				rstr_append_with_char(ctx->head, '\n');

				// .balign	[?]
				if (align > 0) {
					rstr_appendf(ctx->head, ".balign %d\n", align);
				}

				rstr_append_with_raw(ctx->head, node_identifier->token->content, node_identifier->token->len);
				rstr_append_with_char(ctx->head, ':');
				rstr_append_with_char(ctx->head, '\n');

				switch (node_type->type) {
					case BE_NODE_TYPE_CHAR: {
						rstr_append_with_cstr(ctx->head, ".byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT8: {
						rstr_append_with_cstr(ctx->head, ".byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT8: {
						rstr_append_with_cstr(ctx->head, ".byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT16: {
						rstr_append_with_cstr(ctx->head, ".2byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT16: {
						rstr_append_with_cstr(ctx->head, ".2byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT32: {
						rstr_append_with_cstr(ctx->head, ".4byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT32: {
						rstr_append_with_cstr(ctx->head, ".4byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_INT64: {
						rstr_append_with_cstr(ctx->head, ".8byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_UINT64: {
						rstr_append_with_cstr(ctx->head, ".8byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_FLOAT: {
						rstr_append_with_cstr(ctx->head, ".float ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_DOUBLE: {
						rstr_append_with_cstr(ctx->head, ".double ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					case BE_NODE_TYPE_STRUCT: {
						rstr_appendf(
							ctx->head,
							".zero %zu\n",
							be_sem_calc_type_size(ctx->psrctx, node, node_type)
						);
						break;
					}
					case BE_NODE_TYPE_ARRAY: {
						rstr_appendf(
							ctx->head,
							".zero %zu\n",
							be_sem_calc_type_size(ctx->psrctx, node, node_type)
						);
						break;
					}
					case BE_NODE_TYPE_POINTER: {
						rstr_append_with_cstr(ctx->head, ".8byte ");
						_asm_global_var_initializer(ctx, ctx->head, node_expr, node_type);
						break;
					}
					default: {
						assert(0);
						break;
					}
				}

				rstr_free(&rstr_var_name);

				rstr_append_with_cstr(ctx->head, "\n\n");

				break;
			}
			case BE_VAR_TYPE_LOCAL: {
				// TODO: ...
				break;
			}
			case BE_VAR_TYPE_STRUCT_MEMBER: {
				assert(0);
				break;
			}
			default: {
				assert(0);
			}
		}
	}
}







static void _asm_func(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_FUNC);

	ctx->func_symbol = BE_FUNC_AST_NODE_GET_SYMBOL(node);
	assert(ctx->func_symbol);

	ParserASTNode *node_identifier = NULL;
	ParserASTNode *node_func_params = NULL;
	ParserASTNode *node_return_type = NULL;
	ParserASTNode *node_body = NULL;

	int nchilds = node->nchilds;
	if (nchilds == 3) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_body = node->childs[2];
	} else if (nchilds == 4) {
		node_identifier = node->childs[0];
		node_func_params = node->childs[1];
		node_return_type = node->childs[2];
		node_body = node->childs[3];
	} else {
		assert(0);
	}
	assert(node_identifier->type == BE_NODE_IDENTIFIER);
	assert(node_func_params->type == BE_NODE_FUNC_PARAMS);
	if (node_func_params->nchilds > 0) {
		assert(node_func_params->childs[0]->type == BE_NODE_FUNC_PARAMS_ITEM);
	}

	rstr_append_with_char(ctx->body, '\n');

	if (node_body->type == BE_NODE_STATS_BLOCK) {
		// .section	__TEXT,__text,regular,pure_instructions
		rstr_append_with_cstr(ctx->body, ".section	__TEXT,__text,regular,pure_instructions\n");

		// .globl _FUNCTION_NAME
		rstr_append_with_cstr(ctx->body, ".globl ");
		rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);
		rstr_append_with_char(ctx->body, '\n');

		// .p2align	4, 0x90
		// TODO: ...

		// _FUNCTION_NAME:
		rstr_append_with_raw(ctx->body, node_identifier->token->content, node_identifier->token->len);
		rstr_append_with_char(ctx->body, ':');
		rstr_append_with_char(ctx->body, '\n');

		// pushq %rbp
		// movq %rsp, %rbp
		rstr_append_with_cstr(ctx->body, "pushq %rbp\nmovq %rsp, %rbp\n\n");

		size_t address = 0;
		size_t total_size = 0;
		for (int i = 0; i < node_func_params->nchilds; i++) {
			ParserASTNode *node_param = node_func_params->childs[i];
			if (node_param->type == BE_NODE_FUNC_PARAMS_ELLIPSIS_ITEM) {
				continue;
			}

			ParserASTNode *node_param_type = node_param->childs[1];
			ParserSymbol *symbol = BE_FUNC_PARAM_AST_NODE_GET_SYMBOL(node_param);

			int align = BE_VAR_SYMBOL_GET_ALIGN(symbol);
			assert(align);

			size_t type_size = BE_VAR_SYMBOL_GET_TYPE_SIZE(symbol);
			assert(type_size != 0);

			if (address % align > 0) {
				size_t align_padding = align - address % align;
				address += align_padding;
				total_size += align_padding;
			}

			BE_VAR_SYMBOL_SET_ADDRESS(symbol, address);

			address += type_size;
			total_size += type_size;
		}



		// popq	%rbp
		// retq
		rstr_append_with_cstr(ctx->body, "\npopq %rbp\nretq\n");

		rstr_append_with_char(ctx->body, '\n');

	} else if (node_body->type == BE_NODE_DUMMY) {
		return;
	} else {
		assert(0);
	}
}






static void _asm_module_item(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);

	switch (node->type) {
		case BE_NODE_VAR: {
			_asm_var(ctx, node);
			break;
		}
		case BE_NODE_STRUCT: {
			// _asm_struct(ctx, node);
			break;
		}
		case BE_NODE_FUNC: {
			_asm_func(ctx, node);
			break;
		}
		default: {
			assert(0);
			break;
		}
	}
}

static void _asm_module(
	ASMGeneratorGas64Context *ctx,
	ParserASTNode *node
) {
	assert(ctx);
	assert(node);
	assert(node->type == BE_NODE_MODULE);

	_OUT_CSTR(ctx, RSTR_CSTR(ctx->global));

	for (int i = 0; i < node->nchilds; i++) {
		_asm_module_item(ctx, node->childs[i]);
	}

}




void be_asmgen_gas64_generate(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);
	assert(ctx->psrctx);
	assert(ctx->psrctx->ast);
	assert(ctx->psrctx->ast->type == BE_NODE_MODULE);
	assert(ctx->out);

	_asm_module(ctx, ctx->psrctx->ast);

	_OUT_CSTR(ctx, RSTR_CSTR(ctx->head));
	_OUT_CSTR(ctx, RSTR_CSTR(ctx->body));
	_OUT_CSTR(ctx, RSTR_CSTR(ctx->foot));
}

ASMGeneratorGas64Context * be_asmgen_gas64_new_context(
	ParserContext *psrctx,
	FILE *out
) {
	assert(psrctx);
	assert(psrctx->ast);
	assert(out);
	
	ASMGeneratorGas64Context *ctx = (ASMGeneratorGas64Context *)malloc(sizeof(ASMGeneratorGas64Context));
	if (ctx == NULL) {
		_error("cannot allocate memory for a new context!");
	}

	ctx->psrctx = psrctx;
	ctx->out = out;

	ctx->global = rstr_new();
	ctx->head = rstr_new();
	ctx->body = rstr_new();
	ctx->foot = rstr_new();
	ctx->local_var_defs = NULL;

	ctx->local_var_address_counter = 0;
	ctx->local_var_size = 0;

	rstr_init(ctx->global);
	rstr_init(ctx->head);
	rstr_init(ctx->body);
	rstr_init(ctx->foot);

	ctx->counter = 0;
	ctx->func_symbol = NULL;

	return ctx;
}

void be_asmgen_gas64_free_context(
	ASMGeneratorGas64Context *ctx
) {
	assert(ctx);
	assert(ctx->global);
	assert(ctx->head);
	assert(ctx->body);
	assert(ctx->foot);

	rstr_free(ctx->global);
	rstr_free(ctx->head);
	rstr_free(ctx->body);
	rstr_free(ctx->foot);

	free(ctx->global);
	free(ctx->head);
	free(ctx->body);
	free(ctx->foot);

	ctx->global = NULL;
	ctx->head = NULL;
	ctx->body = NULL;
	ctx->foot = NULL;

	free(ctx);
}
