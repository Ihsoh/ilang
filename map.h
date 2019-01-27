#ifndef __MAP_H__
#define __MAP_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#include "list.h"

typedef bool (* MapComparer)(void *a, void *b);
typedef void (* MapReleaser)(void *a);

typedef struct {
	LinkedList		llst;
	MapComparer		key_comparer;
	MapComparer		value_comparer;
	MapReleaser		key_releaser;
	MapReleaser		value_releaser;
} Map;

typedef struct {
	Map		*map;
	void	*key;
	void	*value;
} MapEntry;

typedef struct {
	LinkedListNode	*node;
} MapEntryIterator;

extern void map_init(
	Map *map,
	MapComparer key_comparer,
	MapComparer value_comparer,
	MapReleaser key_releaser,
	MapReleaser value_releaser
);
extern void map_free(
	Map *map,
	bool free_key,
	bool free_value
);
extern void map_set(
	Map *map,
	void *key,
	void *value,
	bool free_old_value
);
extern MapEntry * map_get_entry(
	Map *map,
	void *key
);
extern void * map_get(
	Map *map,
	void *key
);
extern bool map_remove(
	Map *map,
	void *key,
	bool free_key,
	bool free_value
);
extern bool map_exists_key(
	Map *map,
	void *key
);

extern void map_iter_init(
	Map *map,
	MapEntryIterator *iter
);
extern MapEntry * map_iter_next(
	Map *map,
	MapEntryIterator *iter
);

extern bool map_str_comparer(void *a, void *b);
extern bool map_rstr_comparer(void *a, void *b);




extern void map_test(void);

#endif
