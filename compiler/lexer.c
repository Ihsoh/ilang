#include "lexer.h"

#include <stddef.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "../lexer.h"
#include "../util.h"
#include "../string.h"

#include "lexcmd.h"

#define	_COMMON_LEXER_MATCHER(name)	\
	LexerToken * common_lexer_matcher_##name(	\
		LexerContext *ctx,	\
		CommonLexerContext *cmn_ctx,	\
		int type	\
	) {	\
		assert(ctx);	\
		assert(cmn_ctx);	\
		assert(cmn_ctx->is_delimiter);

#define	_COMMON_LEXER_MATCHER_END	\
	}

/*
	捕获空白字符。
*/
_COMMON_LEXER_MATCHER(space)
	if (isspace(lexer_next_char(ctx))) {
		return lexer_new_token(ctx, type);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	捕获单行注释。
*/
_COMMON_LEXER_MATCHER(single_line_comment)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '/') {
		lexer_init_token(ctx, &token, type);

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '/', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\n' && chr != -1, {
				// MATCHED
			}, {
				// NOT MATCHED
				return lexer_clone_token(ctx, &token);
			})
		}

		assert(0);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	多行注释。
*/
_COMMON_LEXER_MATCHER(multiline_comment)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '/') {
		lexer_init_token(ctx, &token, type);

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '*', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '*', {
				// MATCHED
				if (chr == -1) {
					return NULL;
				}
			}, {
				// NOT MATCHED
				LEXER_NEXT_CHAR(ctx, &token)
				LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '/', {
					// MATCHED
					return lexer_clone_token(ctx, &token);
				}, {
					// NOT MATCHED
					if (chr == -1) {
						return NULL;
					}
				})
			})
		}

		return NULL;
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	#...\n
*/
_COMMON_LEXER_MATCHER(preprocess)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '#') {
		lexer_init_token(ctx, &token, type);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr != '\n' && chr != -1, {
				// MATCHED
			}, {
				// NOT MATCHED

				char file[1024];
				snprintf(
					file, sizeof(file),
					"LEXCMD(%s:%d,%d)",
					token.file, token.line, token.column
				);
				LexerContext *ctx_lexcmd = common_lexcmd_new_context(
					file, token.content, token.len
				);

				lexer_parse(ctx_lexcmd);

				LexerToken *tk_cmd = lexer_next_token(ctx_lexcmd);
				assert(tk_cmd);

				if (tk_cmd->type == COMMON_LEXCMD_TOKEN_KEYWORD_LINE) {
					LexerToken *tk_line = lexer_next_token(ctx_lexcmd);
					if (tk_line == NULL
							|| tk_line->type != COMMON_LEXCMD_TOKEN_LITERAL_UINT_DEC) {
						ctx->invalid_token(ctx);
					}

					LexerToken *tk_filename = lexer_next_token(ctx_lexcmd);
					if (tk_filename != NULL
							&& tk_filename->type != COMMON_LEXCMD_TOKEN_LITERAL_STRING) {
						ctx->invalid_token(ctx);
					}

					uint32_t new_line = common_lexcmd_get_uint32_val(ctx_lexcmd, tk_line);
					if (chr == '\n') {
						new_line--;
					}
					LEXER_SET_LINE(new_line)	
					if (tk_filename != NULL) {
						lexer_change_file_by_token(ctx, tk_filename);
					}				

					if (lexer_next_token(ctx_lexcmd) != NULL) {
						ctx->invalid_token(ctx);
					}
				} else {
					ctx->invalid_token(ctx);
				}

				common_lexcmd_free_context(ctx_lexcmd);

				return lexer_clone_token(ctx, &token);
			})
		}

		assert(0);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	捕获二进制数字字面量。
*/
_COMMON_LEXER_MATCHER(literal_uint_bin)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, type);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'b', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '0' || chr == '1', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, chr == '0' || chr == '1' || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (chr == 'u') {
					lexer_next_char(ctx);
					token.len++;
					return lexer_clone_token(ctx, &token);
				} else if (cmn_ctx->is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	捕获八进制数字字面量。
*/
_COMMON_LEXER_MATCHER(literal_uint_oct)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, type);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'o', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr >= '0' && chr <= '7', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '7') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (chr == 'u') {
					lexer_next_char(ctx);
					token.len++;
					return lexer_clone_token(ctx, &token);
				} else if (cmn_ctx->is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	捕获十进制数字字面量。
*/
_COMMON_LEXER_MATCHER(literal_uint_dec)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr >= '0' && chr <= '9') {
		lexer_init_token(ctx, &token, type);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '9') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (chr == 'u') {
					lexer_next_char(ctx);
					token.len++;
					return lexer_clone_token(ctx, &token);
				} else if (cmn_ctx->is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	捕获十六进制数字字面量。
*/
_COMMON_LEXER_MATCHER(literal_uint_hex)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr == '0') {
		lexer_init_token(ctx, &token, type);
		
		LEXER_MATCH_CHAR(ctx, &chr, &token, chr == 'x', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		#define	_IS_HEX_DIGIT_CHAR(chr)	(((chr) >= '0' && (chr) <= '9') || ((chr) >= 'a' && (chr) <= 'f') || ((chr) >= 'A' && (chr) <= 'F'))

		LEXER_MATCH_CHAR(ctx, &chr, &token, _IS_HEX_DIGIT_CHAR(chr) || chr == '_', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, _IS_HEX_DIGIT_CHAR(chr) || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (chr == 'u') {
					lexer_next_char(ctx);
					token.len++;
					return lexer_clone_token(ctx, &token);
				} else if (cmn_ctx->is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		#undef	_IS_HEX_DIGIT_CHAR

		assert(0);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	捕获实型字面量。
*/
_COMMON_LEXER_MATCHER(literal_real)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	if (chr >= '0' && chr <= '9') {
		lexer_init_token(ctx, &token, type);

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '9') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (chr == '.') {
					lexer_next_char(ctx);
					token.len++;
					break;
				} else {
					return NULL;
				}
			})
		}

		LEXER_MATCH_CHAR(ctx, &chr, &token, chr >= '0' && chr <= '9', {
			// MATCHED
		}, {
			// NOT MATCHED
			return NULL;
		})

		for (;;) {
			LEXER_MATCH_CHAR(ctx, &chr, &token, (chr >= '0' && chr <= '9') || chr == '_', {
				// MATCHED
			}, {
				// NOT MATCHED
				if (chr == 'f') {
					lexer_next_char(ctx);
					token.len++;
					return lexer_clone_token(ctx, &token);
				} else if (cmn_ctx->is_delimiter(chr)) {
					return lexer_clone_token(ctx, &token);
				} else {
					return NULL;
				}
			})
		}

		assert(0);
	} else {
		return NULL;
	}
_COMMON_LEXER_MATCHER_END

/*
	捕获字符字面量。
*/
_COMMON_LEXER_MATCHER(literal_char)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“'”。
	if (chr == '\'') {
		lexer_init_token(ctx, &token, type);

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
			// 如果字符不为字符常量所允许的字符则不匹配。
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
_COMMON_LEXER_MATCHER_END

/*
	捕获字符串字面量。
*/
_COMMON_LEXER_MATCHER(literal_string)
	LexerToken token;

	char chr = lexer_next_char(ctx);
	// 捕获“"”。
	if (chr == '"') {
		lexer_init_token(ctx, &token, type);

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
_COMMON_LEXER_MATCHER_END
