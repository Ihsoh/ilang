#include "list.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "myassert.h"

static bool _invalid_comparer(void *node_data, void *data) {
	assert(0);
	return false;
}

static void _invalid_releaser(void *a) {
	assert(0);
}

void llst_init(
	LinkedList *llst,
	LinkedListComparer comparer,
	LinkedListReleaser releaser
) {
	assert(llst);

	llst->head = NULL;
	llst->tail = NULL;
	llst->count = 0;

	if (comparer == NULL) {
		llst->comparer = _invalid_comparer;
	} else {
		llst->comparer = comparer;
	}

	if (releaser == NULL) {
		llst->releaser = _invalid_releaser;
	} else {
		llst->releaser = releaser;
	}
}

void llst_free(
	LinkedList *llst,
	bool free_data
) {
	assert(llst);

	LinkedListNode *node = llst->head;
	while (node != NULL) {
		if (free_data
				&& node->data != NULL) {
			llst->releaser(node->data);
			node->data = NULL;
		}
		LinkedListNode *tmp = node;
		node = node->next;
		free(tmp);
	}
}

void llst_append(
	LinkedList *llst,
	void *data
) {
	LinkedListNode *node = (LinkedListNode *)malloc(sizeof(LinkedListNode));
	node->llst = llst;
	node->data = data;

	if (llst->head == NULL && llst->tail == NULL) {
		llst->head = node;
		llst->tail = node;
		node->previous = NULL;
		node->next = NULL;
	} else if (llst->head != NULL && llst->tail != NULL) {
		llst->tail->next = node;
		node->previous = llst->tail;
		node->next = NULL;
		llst->tail = node;
	} else {
		assert(0);
	}

	llst->count++;
}

void llst_remove(
	LinkedList *llst,
	LinkedListNode *node
) {
	assert(llst);
	assert(node);
	assert(node->llst == llst);

	if (llst->head == node && llst->tail == node) {
		llst->head = NULL;
		llst->tail = NULL;
	} else if (llst->head == node && llst->tail != node) {
		node->next->previous = NULL;
		llst->head = node->next;
	} else if (llst->head != node && llst->tail == node) {
		node->previous->next = NULL;
		llst->tail = node->previous;
	} else if (llst->head != node && llst->tail != node) {
		node->previous->next = node->next;
		node->next->previous = node->previous;
	} else {
		assert(0);
	}

	llst->count--;
	node->llst = NULL;
	node->previous = NULL;
	node->next = NULL;
}

void llst_delete(
	LinkedList *llst,
	LinkedListNode *node,
	bool free_data
) {
	assert(llst);
	assert(node);

	llst_remove(llst, node);
	if (free_data
			&& node->data != NULL) {
		llst->releaser(node->data);
	}
	free(node);
}

LinkedListNode *llst_find_with_comparer(
	LinkedList *llst,
	LinkedListComparer comparer,
	void *data
) {
	assert(llst);
	assert(comparer);
	assert(data);

	LinkedListNode *node = llst->head;
	while (node != NULL) {
		if (comparer(node->data, data)) {
			return node;
		}
		node = node->next;
	}

	return NULL;
}

LinkedListNode *llst_find(
	LinkedList *llst,
	void *data
) {
	assert(llst);
	assert(llst->comparer);
	assert(data);

	return llst_find_with_comparer(llst, llst->comparer, data);
}

static bool _test_comparer(void *node_data, void *data) {
	return strcmp(node_data, data) == 0;
}

void llst_test(void) {
	LinkedList llst;

	llst_init(&llst, _test_comparer, NULL);
	MYASSERT(
		llst.head == NULL && llst.tail == NULL && llst.count == 0,
		"TEST llst_init(...)"
	);

	llst_append(&llst, "A");
	llst_append(&llst, "B");
	llst_append(&llst, "C");
	llst_append(&llst, "D");
	MYASSERT(
		llst.count == 4
			&& llst.head != NULL
			&& strcmp(llst.head->data, "A") == 0
			&& llst.head->next != NULL
			&& strcmp(llst.head->next->data, "B") == 0
			&& llst.head->next->next != NULL
			&& strcmp(llst.head->next->next->data, "C") == 0
			&& llst.head->next->next->next != NULL
			&& strcmp(llst.head->next->next->next->data, "D") == 0,
		"TEST llst_append(...)"
	);

	LinkedListNode *node = llst_find(&llst, "C");
	MYASSERT(
		node != NULL
			&& strcmp(node->data, "C") == 0,
		"TEST llst_find(...)"
	);

	llst_delete(&llst, llst.head->next, false);
	MYASSERT(
		llst.count == 3
			&& llst.head != NULL
			&& strcmp(llst.head->data, "A") == 0
			&& llst.head->next != NULL
			&& strcmp(llst.head->next->data, "C") == 0
			&& llst.head->next->next != NULL
			&& strcmp(llst.head->next->next->data, "D") == 0,
		"TEST llst_delete(...)"
	);

	llst_delete(&llst, llst.head, false);
	MYASSERT(
		llst.count == 2
			&& llst.head != NULL
			&& strcmp(llst.head->data, "C") == 0
			&& llst.head->next != NULL
			&& strcmp(llst.head->next->data, "D") == 0,
		"TEST llst_delete(...)"
	);


	llst_delete(&llst, llst.tail, false);
	MYASSERT(
		llst.count == 1
			&& llst.head != NULL
			&& strcmp(llst.head->data, "C") == 0,
		"TEST llst_delete(...)"
	);

	llst_delete(&llst, llst.head, false);
	MYASSERT(
		llst.head == NULL && llst.tail == NULL && llst.count == 0,
		"TEST llst_delete(...)"
	);

	llst_free(&llst, false);
}
