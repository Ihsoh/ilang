#ifndef	__PREPROCESSOR_GEN_H__
#define	__PREPROCESSOR_GEN_H__

#include <inttypes.h>
#include <stdio.h>

#include "../../parser.h"
#include "../../string.h"
#include "../../map.h"
#include "../../list.h"

typedef struct {
	LinkedList			params;
	ResizableString		body;
} PPGeneratorFuncMacro;

typedef struct {
	ParserContext 		*psrctx;
	FILE 				*out;
	ResizableString		*result;

	/*
		#define	A	1
		#define	B	2

		1、key和value均为ResizableString *类型，这些字符串均使用rstr_new(...)构造。
		2、释放map时需要先将key和value使用rstr_delete(...)释放，再使用map_free(..., false, false)释放map占用的资源。
	*/
	Map					*map_kv;

	/*
		#define	C(a)	(a + 1)
		#define	D(a, b)	(a + b)

		1、key为ResizableString *类型，value为PPGeneratorFuncMacro *类型。
	*/
	Map					*map_kv_with_params;
	
	/*
		cstr_include_paths保存以“;”分隔的include path字符串。
		例子：
			"/A;/B;/C/D"
	*/
	const char			*cstr_include_paths;

	/*
		使用“;”分隔cstr_include_paths。
		例子：
			"/A;/B;/C/D" => ["/A", "/B", "/C/D"]
	*/
	LinkedList			include_paths;
} PPGeneratorContext;

extern void pp_gen_generate(PPGeneratorContext *ctx);
extern PPGeneratorContext * pp_gen_new_context(
	ParserContext *psrctx,
	const char *include_paths,
	Map *map_kv,
	Map *map_kv_with_params,
	FILE *out,
	ResizableString *result
);
extern void pp_gen_free_context(PPGeneratorContext *ctx);

extern void pp_gen_define(
	PPGeneratorContext *ctx,
	ResizableString *k,
	ResizableString *v
);

#endif
