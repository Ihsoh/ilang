#include "map.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "myassert.h"
#include "list.h"
#include "string.h"

static bool _invalid_comparer(void *node_data, void *data) {
	assert(0);
	return false;
}

static void _invalid_releaser(void *a) {
	assert(0);
}

void map_init(
	Map *map,
	MapComparer key_comparer,
	MapComparer value_comparer,
	MapReleaser key_releaser,
	MapReleaser value_releaser
) {
	assert(map);

	llst_init(&(map->llst), NULL, free);

	if (key_comparer == NULL) {
		map->key_comparer = _invalid_comparer;
	} else {
		map->key_comparer = key_comparer;
	}
	if (value_comparer == NULL) {
		map->value_comparer = _invalid_comparer;
	} else {
		map->value_comparer = value_comparer;
	}

	if (key_releaser == NULL) {
		map->key_releaser = _invalid_releaser;
	} else {
		map->key_releaser = key_releaser;
	}
	if (value_releaser == NULL) {
		map->value_releaser = _invalid_releaser;
	} else {
		map->value_releaser = value_releaser;
	}
}

void map_free(
	Map *map,
	bool free_key,
	bool free_value
) {
	if (free_key || free_value) {
		LinkedListNode *node = map->llst.head;
		while (node != NULL) {
			MapEntry *entry = (MapEntry *) node->data;

			if (free_key
					&& entry->key != NULL) {
				map->key_releaser(entry->key);
				entry->key = NULL;
			}

			if (free_value
					&& entry->value != NULL) {
				map->value_releaser(entry->value);
				entry->value = NULL;
			}

			node = node->next;
		}
	}

	llst_free(&(map->llst), true);
	map->key_comparer = NULL;
	map->value_comparer = NULL;
	map->key_releaser = NULL;
	map->value_releaser = NULL;
}

static bool _key_comparer(void *node_data, void *data) {
	assert(node_data);
	assert(data);

	MapEntry *entry = (MapEntry *) node_data;
	Map *map = entry->map;
	return map->key_comparer(entry->key, data);
}

void map_set(
	Map *map,
	void *key,
	void *value,
	bool free_old_value
) {
	assert(map);
	assert(key);

	LinkedList *llst = &(map->llst);
	LinkedListNode *node = llst_find_with_comparer(llst, _key_comparer, key);
	if (node != NULL) {
		MapEntry *entry = (MapEntry *) node->data;

		if (free_old_value
				&& entry->value != NULL) {
			map->value_releaser(entry->value);
		}

		entry->value = value;
	} else {
		MapEntry *entry = (MapEntry *) malloc(sizeof(MapEntry));

		entry->map = map;
		entry->key = key;
		entry->value = value;

		llst_append(llst, entry);
	}
}

static LinkedListNode * _get_node(
	Map *map,
	void *key
) {
	assert(map);
	assert(key);

	LinkedList *llst = &(map->llst);
	LinkedListNode *node = llst_find_with_comparer(llst, _key_comparer, key);
	return node;
}

MapEntry * map_get_entry(
	Map *map,
	void *key
) {
	assert(map);
	assert(key);

	LinkedListNode *node = _get_node(map, key);
	if (node != NULL) {
		return (MapEntry *) node->data;
	} else {
		return NULL;
	}
}

void * map_get(
	Map *map,
	void *key
) {
	assert(map);
	assert(key);

	MapEntry *entry = map_get_entry(map, key);
	if (entry != NULL) {
		return entry->value;
	} else {
		return NULL;
	}
}

bool map_remove(
	Map *map,
	void *key,
	bool free_key,
	bool free_value
) {
	assert(map);
	assert(key);

	LinkedListNode *node = _get_node(map, key);
	if (node != NULL) {
		MapEntry *entry = (MapEntry *) node->data;
		if (free_key
				&& entry->key != NULL) {
			map->key_releaser(entry->key);
		}
		if (free_value
				&& entry->value != NULL) {
			map->value_releaser(entry->value);
		}
		llst_delete(&(map->llst), node, true);
		return true;
	} else {
		return false;
	}
}

bool map_exists_key(
	Map *map,
	void *key
) {
	assert(map);
	assert(key);

	MapEntry *entry = map_get_entry(map, key);

	return entry != NULL;
}




void map_iter_init(
	Map *map,
	MapEntryIterator *iter
) {
	assert(map);
	assert(iter);

	iter->node = map->llst.head;
}

MapEntry * map_iter_next(
	Map *map,
	MapEntryIterator *iter
) {
	assert(map);
	assert(iter);

	if (iter->node == NULL) {
		return NULL;
	}

	LinkedListNode *current = iter->node;
	iter->node = current->next;

	return (MapEntry *)current->data;
}




bool map_str_comparer(void *a, void *b) {
	return strcmp(a, b) == 0;
}

bool map_rstr_comparer(void *a, void *b) {
	return rstr_compare((ResizableString *)a, (ResizableString *)b);
}




static bool _test_str_comparer(void *a, void *b) {
	return strcmp(a, b) == 0;
}

void map_test(void) {
	Map map;

	map_init(&map, _test_str_comparer, _test_str_comparer, free, free);

	map_set(&map, "A", "1", false);
	map_set(&map, "B", "2", false);
	map_set(&map, "C", "3", false);

	void *value = NULL;

	value = map_get(&map, "A");
	MYASSERT(
		value != NULL && strcmp(value, "1") == 0,
		"TEST map"
	);
	value = map_get(&map, "B");
	MYASSERT(
		value != NULL && strcmp(value, "2") == 0,
		"TEST map"
	);
	value = map_get(&map, "C");
	MYASSERT(
		value != NULL && strcmp(value, "3") == 0,
		"TEST map"
	);
	value = map_get(&map, "__NOT_EXISTS__");
	MYASSERT(
		value == NULL,
		"TEST map"
	);

	map_set(&map, "B", "20", false);

	value = map_get(&map, "B");
	MYASSERT(
		value != NULL && strcmp(value, "20") == 0,
		"TEST map"
	);

	MYASSERT(
		map_exists_key(&map, "B"),
		"TEST map"
	);

	map_remove(&map, "B", false, false);

	MYASSERT(
		!map_exists_key(&map, "B"),
		"TEST map"
	);

	map_free(&map, false, false);
}
