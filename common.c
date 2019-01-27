#include "common.h"

#include <assert.h>

#include "string.h"

void cmn_print_token(
	LexerToken *token,
	FILE *out
) {
	for (int32_t i = 0; i < token->len; i++) {
		fputc(token->content[i], out);
	}
}

void cmn_print_code_by_token(
	FILE *out,
	LexerToken *token,
	int n_prev,
	int n_succ
) {
	assert(out);
	assert(token);

	fputc('\n', out);

	ResizableString rstr_line_code;
	ResizableString rstr_line_wavy_line;

	rstr_init(&rstr_line_code);
	rstr_init(&rstr_line_wavy_line);

	LexerToken *t = NULL;

	t = token->prev;
	for (int i = 0; i < n_prev && t != NULL; i++, t = t->prev) {
		rstr_prepend_with_char(&rstr_line_code, ' ');
		rstr_prepend_with_raw(&rstr_line_code, t->content, t->len);

		for (int j = 0; j < t->len + 1; j++) {
			rstr_append_with_char(&rstr_line_wavy_line, ' ');
		}
	}

	rstr_append_with_raw(&rstr_line_code, token->content, token->len);
	for (int i = 0; i < token->len; i++) {
		rstr_append_with_char(&rstr_line_wavy_line, '~');
	}

	t = token->next;
	for (int i = 0; i < n_succ && t != NULL; i++, t = t->next) {
		rstr_append_with_char(&rstr_line_code, ' ');
		rstr_append_with_raw(&rstr_line_code, t->content, t->len);
	}

	fprintf(out, "\t%s\n", RSTR_CSTR(&rstr_line_code));
	fprintf(out, "\t%s\n", RSTR_CSTR(&rstr_line_wavy_line));

	rstr_free(&rstr_line_code);
	rstr_free(&rstr_line_wavy_line);

	fputc('\n', out);
}
