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

LEXER_MATCHER(space)
	*abandon = 0;

	if (isspace(lexer_next_char(ctx))) {
		return lexer_new_token(ctx, PP_TOKEN_SPACE);
	} else {
		return NULL;
	}
LEXER_MATCHER_END

static const char directive_include[] = "include";
static const char directive_define[] = "define";
static const char directive_undef[] = "undef";
static const char directive_if[] = "if";
static const char directive_ifdef[] = "ifdef";
static const char directive_ifndef[] = "ifndef";
static const char directive_elif[] = "elif";
static const char directive_elifdef[] = "elifdef";
static const char directive_elifndef[] = "elifndef";
static const char directive_else[] = "else";
static const char directive_endif[] = "endif";
static const char directive_info[] = "info";
static const char directive_warn[] = "warn";
static const char directive_error[] = "error";

#define	DIRECTIVE_LEN_INCLUDE		(sizeof(directive_include) - 1)
#define	DIRECTIVE_LEN_DEFINE		(sizeof(directive_define) - 1)
#define	DIRECTIVE_LEN_UNDEF			(sizeof(directive_undef) - 1)
#define	DIRECTIVE_LEN_IF			(sizeof(directive_if) - 1)
#define	DIRECTIVE_LEN_IFDEF			(sizeof(directive_ifdef) - 1)
#define	DIRECTIVE_LEN_IFNDEF		(sizeof(directive_ifndef) - 1)
#define	DIRECTIVE_LEN_ELIF			(sizeof(directive_elif) - 1)
#define	DIRECTIVE_LEN_ELIFDEF		(sizeof(directive_elifdef) - 1)
#define	DIRECTIVE_LEN_ELIFNDEF		(sizeof(directive_elifndef) - 1)
#define	DIRECTIVE_LEN_ELSE			(sizeof(directive_else) - 1)
#define	DIRECTIVE_LEN_ENDIF			(sizeof(directive_endif) - 1)
#define	DIRECTIVE_LEN_INFO			(sizeof(directive_info) - 1)
#define	DIRECTIVE_LEN_WARN			(sizeof(directive_warn) - 1)
#define	DIRECTIVE_LEN_ERROR			(sizeof(directive_error) - 1)

static bool is_space(char chr) {
	return chr == ' ' || chr == '\t';
}

LEXER_MATCHER(keyword)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);

	if (chr == '#') {
		lexer_init_token(ctx, &token, PP_TOKEN_INVALID);

		#define	__MATCH(t)	{	\
			token.type = (t);	\
			for (;;) {	\
				LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\n' && chr != -1, {	\
				}, {	\
					return lexer_clone_token(ctx, &token);	\
				})	\
			}	\
		}

		if (lexer_match_string(ctx, directive_include)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_INCLUDE + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_INCLUDE)

		} else if (lexer_match_string(ctx, directive_define)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_DEFINE + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_DEFINE)

		} else if (lexer_match_string(ctx, directive_undef)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_UNDEF + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_UNDEF)

		} else if (lexer_match_string(ctx, directive_if)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_IF + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_IF)

		} else if (lexer_match_string(ctx, directive_ifdef)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_IFDEF + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_IFDEF)

		} else if (lexer_match_string(ctx, directive_ifndef)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_IFNDEF + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_IFNDEF)

		} else if (lexer_match_string(ctx, directive_elif)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_ELIF + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_ELIF)

		} else if (lexer_match_string(ctx, directive_elifdef)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_ELIFDEF + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_ELIFDEF)

		} else if (lexer_match_string(ctx, directive_elifndef)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_ELIFNDEF + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_ELIFNDEF)

		} else if (lexer_match_string(ctx, directive_else)) {

			__MATCH(PP_TOKEN_KEYWORD_ELSE)

		} else if (lexer_match_string(ctx, directive_endif)) {
			
			__MATCH(PP_TOKEN_KEYWORD_ENDIF)

		} else if (lexer_match_string(ctx, directive_info)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_INFO + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_INFO)

		} else if (lexer_match_string(ctx, directive_warn)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_WARN + 1))) {
			
			__MATCH(PP_TOKEN_KEYWORD_WARN)

		} else if (lexer_match_string(ctx, directive_error)
				&& is_space(lexer_peek_char(ctx, DIRECTIVE_LEN_ERROR + 1))) {

			__MATCH(PP_TOKEN_KEYWORD_ERROR)

		} else {

			return NULL;
		
		}

		#undef	__MATCH
	} else {
		return NULL;
	}
LEXER_MATCHER_END

LEXER_MATCHER(cmd_line)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);

	if (chr == '#') {
		lexer_init_token(ctx, &token, PP_TOKEN_CMD_LINE);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\n' && chr != -1, {
				// MATCHED
			}, {
				// NOT MATCHED
				return lexer_clone_token(ctx, &token);
			})
		}
	} else {
		return NULL;
	}
LEXER_MATCHER_END

LEXER_MATCHER(line)
	*abandon = 0;

	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (isspace(chr)) {
		return NULL;
	}

	lexer_init_token(ctx, &token, PP_TOKEN_LINE);

	for (;;) {
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\n' && chr != -1, {
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
	_MATCHER_LIST_ITEM(keyword),
	_MATCHER_LIST_ITEM(cmd_line),
	_MATCHER_LIST_ITEM(line),
	{NULL, NULL}
};

#undef	_MATCHER_LIST_ITEM

LexerContext * pp_lexer_new_context(
	const char *file,
	const char *source,
	int len
) {
	assert(file);
	assert(source);
	assert(len >= 0);

	return lexer_new_context(file, source, len, _matchers);
}

void pp_lexer_free_context(LexerContext * ctx) {
	assert(ctx);

	lexer_free_context(ctx);
}
