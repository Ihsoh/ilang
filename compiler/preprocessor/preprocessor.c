#include "preprocessor.h"

#include "../../string.h"
#include "../../map.h"

#include "parser.h"
#include "gen.h"
#include "comment.h"
#include "line_concat.h"

#include <stdio.h>
#include <assert.h>

void pp_process(
	const char *file,
	const char *include_paths,
	const char *source,
	int len,
	Map *defs,
	FILE *out,
	ResizableString *result
) {
	assert(file);
	assert(include_paths);
	assert(source);
	assert(defs);
	assert(out != NULL || result != NULL);

	ResizableString rstr_comment_removed;
	rstr_init(&rstr_comment_removed);
	pp_comment_process(
		&rstr_comment_removed,
		file,
		source,
		len
	);

	ResizableString rstr_line_concated;
	rstr_init(&rstr_line_concated);
	pp_line_concat_process(
		&rstr_line_concated,
		file,
		RSTR_CSTR(&rstr_comment_removed),
		RSTR_LEN(&rstr_comment_removed)
	);

	rstr_free(&rstr_comment_removed);
	
	ParserContext *psrctx = pp_parser_new_context(
		file,
		RSTR_CSTR(&rstr_line_concated),
		RSTR_LEN(&rstr_line_concated)
	);
	pp_parser_parse(psrctx);

	PPGeneratorContext *genctx = pp_gen_new_context(
		psrctx, include_paths, NULL, NULL, out, result
	);

	MapEntryIterator iter;
	map_iter_init(defs, &iter);
	MapEntry *entry = NULL;
	while ((entry = map_iter_next(defs, &iter)) != NULL) {
		pp_gen_define(genctx, entry->key, entry->value);
	}

	pp_gen_generate(genctx);
	pp_gen_free_context(genctx);

	pp_parser_free_context(psrctx);

	rstr_free(&rstr_line_concated);
}
