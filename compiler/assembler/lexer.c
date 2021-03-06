#include "lexer.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../../lexer.h"
#include "../../util.h"
#include "../../string.h"

#include "../lexer.h"

#include "ins.h"

static bool _is_delimiter(int chr) {
	return chr == -1 || !isalpha(chr) || chr != '_';
}

static CommonLexerContext _common_lexer_context = {
	.is_delimiter = _is_delimiter
};

#define	_COMMON_LEXER_MATCHER(name, type)	\
	(common_lexer_matcher_##name(ctx, &_common_lexer_context, (type)))




/*
	捕获空白字符。
*/
LEXER_MATCHER(space)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(space, ASM_TOKEN_SPACE);
LEXER_MATCHER_END

/*
	捕获单行注释。
*/
LEXER_MATCHER(single_line_comment)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(single_line_comment, ASM_TOKEN_SINGLE_LINE_COMMENT);
LEXER_MATCHER_END

/*
	多行注释。
*/
LEXER_MATCHER(multiline_comment)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(multiline_comment, ASM_TOKEN_MULTILINE_COMMENT);
LEXER_MATCHER_END

/*
	#...\n
*/
LEXER_MATCHER(preprocess)
	LEXER_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(preprocess, ASM_TOKEN_PREPROCESS);
LEXER_MATCHER_END

/*
	捕获二进制数字字面量。
*/
LEXER_MATCHER(literal_uint_bin)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_bin, ASM_TOKEN_LITERAL_UINT_BIN);
LEXER_MATCHER_END

/*
	捕获八进制数字字面量。
*/
LEXER_MATCHER(literal_uint_oct)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_oct, ASM_TOKEN_LITERAL_UINT_OCT);
LEXER_MATCHER_END

/*
	捕获十进制数字字面量。
*/
LEXER_MATCHER(literal_uint_dec)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_dec, ASM_TOKEN_LITERAL_UINT_DEC);
LEXER_MATCHER_END

/*
	捕获十六进制数字字面量。
*/
LEXER_MATCHER(literal_uint_hex)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_uint_hex, ASM_TOKEN_LITERAL_UINT_HEX);
LEXER_MATCHER_END

/*
	捕获实型字面量。
*/
LEXER_MATCHER(literal_real)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_real, ASM_TOKEN_LITERAL_REAL);
LEXER_MATCHER_END

/*
	捕获字符字面量。
*/
LEXER_MATCHER(literal_char)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_char, ASM_TOKEN_LITERAL_CHAR);
LEXER_MATCHER_END

/*
	捕获字符串字面量。
*/
LEXER_MATCHER(literal_string)
	LEXER_NO_ABANDON_TOKEN
	return _COMMON_LEXER_MATCHER(literal_string, ASM_TOKEN_LITERAL_STRING);
LEXER_MATCHER_END

static bool _is_mnemonic(
	LexerToken *token
) {
	assert(token);

	InstructionIterator iter;
	ins_iter_init(&iter);
	for (Instruction *ins = ins_iter_next(&iter); ins != NULL; ins = ins_iter_next(&iter)) {
		assert(ins->mnemonic);

		if (strlen(ins->mnemonic) == token->len
				&& strncmp(ins->mnemonic, token->content, token->len) == 0) {
			return true;
		}
	}
	ins_iter_free(&iter);

	return false;
}

static bool _is_register(
	LexerToken *token
) {
	assert(token);

	InsRegisterIterator iter;
	ins_reg_iter_init(&iter);
	for (InsRegister *reg = ins_reg_iter_next(&iter); reg != NULL; reg = ins_reg_iter_next(&iter)) {
		assert(reg->name);

		if (strlen(reg->name) == token->len
				&& strncmp(reg->name, token->content, token->len) == 0) {
			return true;
		}
	}
	ins_reg_iter_free(&iter);

	return false;
}

/*
	捕获关键字。
*/
LEXER_MATCHER(keyword)
	*abandon = 0;

	LexerToken token;

	#define	_KEYWORD(text, type)	{(text), sizeof((text)) - 1, (type)}

	static struct _Keyword {
		const char	*text;
		const int	len;
		const int	type;
	} keywords[] = {
		_KEYWORD("BYTE", ASM_TOKEN_KEYWORD_BYTE),
		_KEYWORD("WORD", ASM_TOKEN_KEYWORD_WORD),
		_KEYWORD("DWORD", ASM_TOKEN_KEYWORD_DWORD),
		_KEYWORD("QWORD", ASM_TOKEN_KEYWORD_QWORD),
		_KEYWORD("ADDR16", ASM_TOKEN_KEYWORD_ADDR16),
		_KEYWORD("ADDR32", ASM_TOKEN_KEYWORD_ADDR32),
		_KEYWORD("ADDR64", ASM_TOKEN_KEYWORD_ADDR64),
		_KEYWORD("HINT", ASM_TOKEN_KEYWORD_HINT),
		_KEYWORD("opcode_o1", ASM_TOKEN_KEYWORD_OPCODE_O1),
		_KEYWORD("opcode_o2", ASM_TOKEN_KEYWORD_OPCODE_O2),
		_KEYWORD("opcode_o3", ASM_TOKEN_KEYWORD_OPCODE_O3),
		_KEYWORD("opcode_len", ASM_TOKEN_KEYWORD_OPCODE_LEN),

		{NULL, 0, ASM_TOKEN_INVALID}
	};

	#undef	_KEYWORD

	char chr = lexer_next_char(ctx);

	if (isalpha(chr) || chr == '_' || chr == '$') {
		lexer_init_token(ctx, &token, ASM_TOKEN_IDENTIFIER);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, isalnum(chr) || chr == '_' || chr == '.' || chr == '$', {
				// MATCHED
			}, {
				// NOT MATCHED
				break;
			})
		}

		for (struct _Keyword *keyword = keywords; keyword->text != NULL; keyword++) {
			if (keyword->len == token.len
					&& strncmp(keyword->text, token.content, keyword->len) == 0) {
				token.type = keyword->type;
				break;
			}
		}

		if (_is_mnemonic(&token)) {
			token.type = ASM_TOKEN_KEYWORD_INSTRUCTION;
		}
		
		if (_is_register(&token)) {
			token.type = ASM_TOKEN_KEYWORD_REGISTER;
		}

		return lexer_clone_token(ctx, &token);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

LEXER_TEXT_MATCHER(pnct_semicolon, ";", ASM_TOKEN_PNCT_SEMICOLON, 0)
LEXER_TEXT_MATCHER(pnct_parentheses_left, "(", ASM_TOKEN_PNCT_PARENTHESES_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_parentheses_right, ")", ASM_TOKEN_PNCT_PARENTHESES_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_colon, ":", ASM_TOKEN_PNCT_COLON, 0)
LEXER_TEXT_MATCHER(pnct_comma, ",", ASM_TOKEN_PNCT_COMMA, 0)
LEXER_TEXT_MATCHER(pnct_ellipsis, "...", ASM_TOKEN_PNCT_ELLIPSIS, 0)
LEXER_TEXT_MATCHER(pnct_bor, "|", ASM_TOKEN_PNCT_BOR, 0)
LEXER_TEXT_MATCHER(pnct_bxor, "^", ASM_TOKEN_PNCT_BXOR, 0)
LEXER_TEXT_MATCHER(pnct_band, "&", ASM_TOKEN_PNCT_BAND, 0)
LEXER_TEXT_MATCHER(pnct_equal, "==", ASM_TOKEN_PNCT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_not_equal, "!=", ASM_TOKEN_PNCT_NOT_EQUAL, 0)
LEXER_TEXT_MATCHER(pnct_lt, "<", ASM_TOKEN_PNCT_LT, 0)
LEXER_TEXT_MATCHER(pnct_le, "<=", ASM_TOKEN_PNCT_LE, 0)
LEXER_TEXT_MATCHER(pnct_gt, ">", ASM_TOKEN_PNCT_GT, 0)
LEXER_TEXT_MATCHER(pnct_ge, ">=", ASM_TOKEN_PNCT_GE, 0)
LEXER_TEXT_MATCHER(pnct_shift_left, "<<", ASM_TOKEN_PNCT_SHIFT_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_shift_right, ">>", ASM_TOKEN_PNCT_SHIFT_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_add, "+", ASM_TOKEN_PNCT_ADD, 0)
LEXER_TEXT_MATCHER(pnct_sub, "-", ASM_TOKEN_PNCT_SUB, 0)
LEXER_TEXT_MATCHER(pnct_mul, "*", ASM_TOKEN_PNCT_MUL, 0)
LEXER_TEXT_MATCHER(pnct_div, "/", ASM_TOKEN_PNCT_DIV, 0)
LEXER_TEXT_MATCHER(pnct_mod, "%", ASM_TOKEN_PNCT_MOD, 0)
LEXER_TEXT_MATCHER(pnct_not, "!", ASM_TOKEN_PNCT_NOT, 0)
LEXER_TEXT_MATCHER(pnct_bnot, "~", ASM_TOKEN_PNCT_BNOT, 0)
LEXER_TEXT_MATCHER(pnct_brackets_left, "[", ASM_TOKEN_PNCT_BRACKETS_LEFT, 0)
LEXER_TEXT_MATCHER(pnct_brackets_right, "]", ASM_TOKEN_PNCT_BRACKETS_RIGHT, 0)
LEXER_TEXT_MATCHER(pnct_or, "||", ASM_TOKEN_PNCT_OR, 0)
LEXER_TEXT_MATCHER(pnct_and, "&&", ASM_TOKEN_PNCT_AND, 0)
LEXER_TEXT_MATCHER(pnct_question_mark, "?", ASM_TOKEN_PNCT_QUESTION_MARK, 0)
LEXER_TEXT_MATCHER(pnct_assign, "=", ASM_TOKEN_PNCT_ASSIGN, 0)




#define	_MATCHER_LIST_ITEM(name)	{LEXER_MATCHER_NAME_S(name), LEXER_MATCHER_NAME(name)}

static LexerTokenMatcher _matchers[] = {
	_MATCHER_LIST_ITEM(space),

	_MATCHER_LIST_ITEM(single_line_comment),
	_MATCHER_LIST_ITEM(multiline_comment),

	_MATCHER_LIST_ITEM(preprocess),

	_MATCHER_LIST_ITEM(literal_uint_bin),
	_MATCHER_LIST_ITEM(literal_uint_oct),
	_MATCHER_LIST_ITEM(literal_uint_dec),
	_MATCHER_LIST_ITEM(literal_uint_hex),
	_MATCHER_LIST_ITEM(literal_real),
	_MATCHER_LIST_ITEM(literal_char),
	_MATCHER_LIST_ITEM(literal_string),

	_MATCHER_LIST_ITEM(keyword),

	_MATCHER_LIST_ITEM(pnct_semicolon),
	_MATCHER_LIST_ITEM(pnct_parentheses_left),
	_MATCHER_LIST_ITEM(pnct_parentheses_right),
	_MATCHER_LIST_ITEM(pnct_colon),
	_MATCHER_LIST_ITEM(pnct_comma),
	_MATCHER_LIST_ITEM(pnct_ellipsis),
	_MATCHER_LIST_ITEM(pnct_bor),
	_MATCHER_LIST_ITEM(pnct_bxor),
	_MATCHER_LIST_ITEM(pnct_band),
	_MATCHER_LIST_ITEM(pnct_equal),
	_MATCHER_LIST_ITEM(pnct_not_equal),
	_MATCHER_LIST_ITEM(pnct_lt),
	_MATCHER_LIST_ITEM(pnct_le),
	_MATCHER_LIST_ITEM(pnct_gt),
	_MATCHER_LIST_ITEM(pnct_ge),
	_MATCHER_LIST_ITEM(pnct_shift_left),
	_MATCHER_LIST_ITEM(pnct_shift_right),
	_MATCHER_LIST_ITEM(pnct_add),
	_MATCHER_LIST_ITEM(pnct_sub),
	_MATCHER_LIST_ITEM(pnct_mul),
	_MATCHER_LIST_ITEM(pnct_div),
	_MATCHER_LIST_ITEM(pnct_mod),
	_MATCHER_LIST_ITEM(pnct_not),
	_MATCHER_LIST_ITEM(pnct_bnot),
	_MATCHER_LIST_ITEM(pnct_brackets_left),
	_MATCHER_LIST_ITEM(pnct_brackets_right),
	_MATCHER_LIST_ITEM(pnct_or),
	_MATCHER_LIST_ITEM(pnct_and),
	_MATCHER_LIST_ITEM(pnct_question_mark),
	_MATCHER_LIST_ITEM(pnct_assign),

	{NULL, NULL}
};

#undef	_MATCHER_LIST_ITEM

LexerContext * asm_lexer_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	return lexer_new_context(file, source, len, _matchers);
}

void asm_lexer_free_context(
	LexerContext * ctx
) {
	assert(ctx);

	lexer_free_context(ctx);
}

uint32_t asm_lexer_get_uint32_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	switch (token->type) {
		case ASM_TOKEN_LITERAL_UINT_BIN: {
			return util_get_uint32_bin_val(token->content, token->len);
		}
		case ASM_TOKEN_LITERAL_UINT_OCT: {
			return util_get_uint32_oct_val(token->content, token->len);
		}
		case ASM_TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint32_dec_val(token->content, token->len);
		}
		case ASM_TOKEN_LITERAL_UINT_HEX: {
			return util_get_uint32_hex_val(token->content, token->len);
		}
		default: {
			assert(0);
		}
	}

	return 0;
}

uint64_t asm_lexer_get_uint64_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	switch (token->type) {
		case ASM_TOKEN_LITERAL_UINT_BIN: {
			return util_get_uint64_bin_val(token->content, token->len);
		}
		case ASM_TOKEN_LITERAL_UINT_OCT: {
			return util_get_uint64_oct_val(token->content, token->len);
		}
		case ASM_TOKEN_LITERAL_UINT_DEC: {
			return util_get_uint64_dec_val(token->content, token->len);
		}
		case ASM_TOKEN_LITERAL_UINT_HEX: {
			return util_get_uint64_hex_val(token->content, token->len);
		}
		default: {
			assert(0);
		}
	}

	return 0;
}

float asm_lexer_get_float_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == ASM_TOKEN_LITERAL_REAL) {
		return util_get_float_val(token->content, token->len);
	} else {
		assert(0);
	}
	
	return 0.0f;
}

double asm_lexer_get_double_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == ASM_TOKEN_LITERAL_REAL) {
		return util_get_double_val(token->content, token->len);
	} else {
		assert(0);
	}
	
	return 0.0;
}

char asm_lexer_get_char_val(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);

	if (token->type == ASM_TOKEN_LITERAL_CHAR) {
		return util_get_char_val(token->content, token->len);
	} else {
		assert(0);
	}

	return '\0';
}

void asm_lexer_unescape_string(
	LexerContext *ctx,
	ResizableString *target,
	const char *source,
	size_t source_len
) {
	assert(ctx);
	assert(target);
	assert(source);

	util_unescape_string(target, source, source_len);
}

bool asm_lexer_has_unsigned_mark(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);
	assert(
		token->type == ASM_TOKEN_LITERAL_UINT_BIN
		|| token->type == ASM_TOKEN_LITERAL_UINT_OCT
		|| token->type == ASM_TOKEN_LITERAL_UINT_DEC
		|| token->type == ASM_TOKEN_LITERAL_UINT_HEX
	);

	return token->content[token->len - 1] == 'u';
}

bool asm_lexer_has_float_mark(
	LexerContext *ctx,
	LexerToken *token
) {
	assert(ctx);
	assert(token);
	assert(token->type == ASM_TOKEN_LITERAL_REAL);

	return token->content[token->len - 1] == 'f';
}
