#ifndef	__LIST_H__
#define	__LIST_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#define	LLST_LEN(llst)	((llst)->count)

typedef bool (* LinkedListComparer)(void *node_data, void *data);
typedef void (* LinkedListReleaser)(void *a);

struct _LinkedListNode;

typedef struct {
	struct _LinkedListNode	*head;
	struct _LinkedListNode	*tail;
	size_t					count;
	LinkedListComparer		comparer;
	LinkedListReleaser		releaser;
} LinkedList;

typedef struct _LinkedListNode {
	LinkedList				*llst;
	void					*data;
	struct _LinkedListNode	*previous;
	struct _LinkedListNode	*next;
} LinkedListNode;

extern void llst_init(
	LinkedList *llst,
	LinkedListComparer comparer,
	LinkedListReleaser releaser
);
extern void llst_free(
	LinkedList *llst,
	bool free_data
);
extern void llst_append(
	LinkedList *llst,
	void *data
);
extern void llst_remove(
	LinkedList *llst,
	LinkedListNode *node
);
extern void llst_delete(
	LinkedList *llst,
	LinkedListNode *node,
	bool free_data
);
extern LinkedListNode *llst_find_with_comparer(
	LinkedList *llst,
	LinkedListComparer comparer,
	void *data
);
extern LinkedListNode *llst_find(
	LinkedList *llst,
	void *data
);




extern void llst_test(void);

#endif
