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
				&& strncmp(ins->mnemonic, token->content, token->len)) {
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
				&& strncmp(reg->name, token->content, token->len)) {
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
