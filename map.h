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

#define	MAP_PRIMITIVE_BOX_TYPE_CHAR			1
#define	MAP_PRIMITIVE_BOX_TYPE_UINT8		2
#define	MAP_PRIMITIVE_BOX_TYPE_UINT16		3
#define	MAP_PRIMITIVE_BOX_TYPE_UINT32		4
#define	MAP_PRIMITIVE_BOX_TYPE_UINT64		5
#define	MAP_PRIMITIVE_BOX_TYPE_INT8			6
#define	MAP_PRIMITIVE_BOX_TYPE_INT16		7
#define	MAP_PRIMITIVE_BOX_TYPE_INT32		8
#define	MAP_PRIMITIVE_BOX_TYPE_INT64		9
#define	MAP_PRIMITIVE_BOX_TYPE_FLOAT		10
#define	MAP_PRIMITIVE_BOX_TYPE_DOUBLE		11

typedef struct {
	uint8_t			type;
	union {
		char		c;
		uint8_t		u8;
		uint16_t	u16;
		uint32_t	u32;
		uint64_t	u64;
		int8_t		i8;
		int16_t		i16;
		int32_t		i32;
		int64_t		i64;
		float		f;
		double		d;
	} value;
} MapPrimitiveBox;

#define	_MAP_PRIMITIVE_BOX_xxxx_DECL(xxxx, param_type)	\
	extern MapPrimitiveBox * map_primitive_box_##xxxx(	\
		param_type value	\
	);

_MAP_PRIMITIVE_BOX_xxxx_DECL(char, char)
_MAP_PRIMITIVE_BOX_xxxx_DECL(uint8, uint8_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(uint16, uint16_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(uint32, uint32_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(uint64, uint64_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(int8, int8_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(int16, int16_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(int32, int32_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(int64, int64_t)
_MAP_PRIMITIVE_BOX_xxxx_DECL(float, float)
_MAP_PRIMITIVE_BOX_xxxx_DECL(double, double)

extern void map_primitive_box_free(
	MapPrimitiveBox *box
);

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
