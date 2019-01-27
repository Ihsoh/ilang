#ifndef __LEXER_H__
#define __LEXER_H__

#include <stdbool.h>
#include <stdio.h>

#include "array.h"

/*
	词法解析器的标记。
	其中content总是指向LexerContext.source所指向的内存的任意合法位置。
*/
typedef struct _LexerToken {
	int					type;			// 标记的类型，自定义。

	int					len;			// 标记的长度，必须大于0才有意义。
	const char			*content;		// 指向标记在LexerContext.source开始的位置。

	/*
		file，line和column记录了一个标记所在的位置，但是在同一个文件里的两个标记，他们不一定拥有相同的文件名，
		甚至在文本中B标记在A标记前方，B的值也有可能比A小，比如下方伪代码：

			1) #line 2 "a.txt"
			2) A
			3) #line 1 "a.txt"
			4) B

		标记A：line=2，column=1。
		标记B：line=1，column=1。
	*/
	const char			*file;			// 标记所存在的文件的文件名，存在于同一个LexerContext实例的不同标记允许有不同的文件名。
	int					line;			// 标记所存在的行号，第一行时该值为0，初始值为0。
	int					column;			// 标记所存在的列号，第一列时该值为0，初始值为-1。

	struct _LexerToken	*prev;			// 标记的前驱。
	struct _LexerToken	*next;			// 标记的后继。

	struct _LexerToken	*tks_next;		// 标记的后继，该后继组成的链表记录了该上下文分配的所有标记。

	void				*matcher_data;	// 匹配器附带数据。
} LexerToken;

#ifndef	LEXER_CONTEXT_CURSTACK_SIZE
	#define	LEXER_CONTEXT_CURSTACK_SIZE		1024
#endif

/*
	词法解析器的上下文。
*/
typedef struct _LexerContext {
	const char			*file;					// 当前所处的文件的文件名，可变。
	ResizableArray		rarr_file;				// 缓存所有由#line指令添加的文件名内存的指针。

	const char			*source;				// 当前正在处理的源代码文本。
	int					len;					// 源代码文本的长度，单位：字节。
	int					index;					// 指向最后一个已被处理的字符的位置，该位置的字符已经被获取。
	int					line;					// 最后一个已被处理的字符的行号。
	int					column;					// 最后一个已被处理的字符的列号。

	int					stored_index;			// 缓存的字符位置。
	int					stored_line;			// 缓存的行号。
	int					stored_column;			// 缓存的列号。
	
	LexerToken			*head;					// 指向标记链表的第一个标记。
	LexerToken			*tail;					// 指向标记链表的最后一个标记。
	LexerToken			*current;				// 指向将要被获取的标记。

	LexerToken			*tokens;				// 指向一个链表的头部，该链表包含了该上下文分配的所有标记。

	int					curstack_top;
	LexerToken			*curstack[LEXER_CONTEXT_CURSTACK_SIZE];

	const struct _LexerTokenMatcher *matchers;	// 标记匹配器列表，需要以“{NULL, NULL}”结束。

	void				*data;

	void (* invalid_token)(struct _LexerContext *ctx);
	void (* alloc_mem_error)(struct _LexerContext *ctx);
} LexerContext;

typedef struct _LexerTokenMatcher {
	const char *name;
	LexerToken * (* match)(LexerContext *ctx, int *abandon);
} LexerTokenMatcher;

extern LexerContext * lexer_new_context(
	const char *file,
	const char *source,
	int len,
	LexerTokenMatcher *matchers
);
extern void lexer_free_context(LexerContext * ctx);

extern int lexer_next_char(LexerContext *ctx);
extern bool lexer_match_string(
	LexerContext *ctx,
	const char *str
);
extern int lexer_peek_char(LexerContext *ctx, int n);

extern void lexer_reset_token(LexerContext *ctx, int head);
extern LexerToken * lexer_next_token(LexerContext *ctx);
extern LexerToken * lexer_prev_token(LexerContext *ctx);
extern void lexer_init_token(LexerContext *ctx, LexerToken *token, int type);
extern LexerToken * lexer_new_token(LexerContext *ctx, int type);
extern LexerToken * lexer_clone_token(LexerContext *ctx, LexerToken *token);
extern void lexer_free_token(LexerContext *ctx, LexerToken *token);
extern void lexer_store_current(LexerContext *ctx);
extern void lexer_resume_current(LexerContext *ctx);
extern void lexer_abandon_curstack_top(LexerContext *ctx);

extern void lexer_parse(LexerContext *ctx);
extern void lexer_parse_first(LexerContext *ctx);

extern bool lexer_compare_token(
	LexerToken *a,
	LexerToken *b
);

extern char * lexer_new_cstr_from_token(LexerToken *token);

extern void lexer_change_file_by_token(
	LexerContext *ctx,
	LexerToken *token
);

#define	LEXER_MATCHER(name) 		static LexerToken * name##_matcher(LexerContext *ctx, int *abandon) {	\
										assert(ctx); assert(abandon); {
#define	LEXER_SET_LINE(ln)			{ ctx->line = ln; }
#define	LEXER_MATCHER_END			} assert(0); }


#define	LEXER_MATCHER_NAME(name)	name##_matcher
#define	LEXER_MATCHER_NAME_S(name)	#name"_matcher"



#define	LEXER_MATCH_CHAR(ctx_ptr, chr_ptr, tk_ptr, cond, matched_body, not_matched_body)	\
	{	\
		*(chr_ptr) = lexer_peek_char((ctx_ptr), 1);	\
		if ((cond)) {	\
			lexer_next_char((ctx_ptr));	\
			(tk_ptr)->len++;	\
			{ matched_body }	\
		} else {	\
			{ not_matched_body }	\
		}	\
	}
#define	LEXER_SKIP_CHAR(ctx_ptr)			{ lexer_next_char((ctx_ptr)); }
#define	LEXER_NEXT_CHAR(ctx_ptr, tk_ptr)	{ lexer_next_char((ctx_ptr)); (tk_ptr)->len++; }

#define	LEXER_TEXT_MATCHER(name, text, type, abd)	\
	LEXER_MATCHER(name)	\
		int len = strlen((text));	\
		assert(len > 0);	\
		*abandon = (abd);	\
		LexerToken token;	\
		char chr = lexer_next_char(ctx);	\
		if (chr == (text)[0]) {	\
			lexer_init_token(ctx, &token, (type));	\
			for (int i = 1; i < len; i++) {	\
				LEXER_MATCH_CHAR(ctx, &chr, &token, chr == (text)[i], {}, { return NULL; })	\
			}	\
			return lexer_clone_token(ctx, &token);	\
		} else {	\
			return NULL;	\
		}	\
	LEXER_MATCHER_END


#endif
